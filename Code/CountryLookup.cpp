
#include "CountryLookup.h"

namespace BC
{
    namespace Services
    {
        void CountryLookup::prerun()
        {
            // Compile list of lists to be downloaded from registrants
            registrantLists[COUNTRYLOOKUP_LIST_RIPENCC] = COUNTRYLOOKUP_LIST_RIPENCC_URL;
            registrantLists[COUNTRYLOOKUP_LIST_LACNIC] = COUNTRYLOOKUP_LIST_LACNIC_URL;
            registrantLists[COUNTRYLOOKUO_LIST_ARIN] = COUNTRYLOOKUP_LIST_ARIN_URL;
            registrantLists[COUNTRYLOOKUP_LIST_APNIC] = COUNTRYLOOKUP_LIST_APNIC_URL;
            registrantLists[COUNTRYLOOKUP_LIST_AFRNIC] = COUNTRYLOOKUP_LIST_AFRINIC_URL;
        }
        void CountryLookup::run()
        {
            // Load configuration
            Configurator *c = static_cast<Configurator*>(controller->getServiceByName(SERVICETITLE_CONFIGURATOR));
            // -- Database
            qs =        "hostaddr=" + c->get(CONFIG__DATABASE_HOST, MODULES_PAGES_DEFAULT_HOST) +
                        " port=" + Utils::intToString(c->get(CONFIG__DATABASE_PORT, MODULES_PAGES_DEFAULT_PORT)) +
                        " dbname=" + c->get(CONFIG__DATABASE_DB, MODULES_PAGES_DEFAULT_DATABASE) +
                        " user=" + c->get(CONFIG__DATABASE_USER, MODULES_PAGES_DEFAULT_USER) +
                        " password=" + c->get(CONFIG__DATABASE_PASS, MODULES_PAGES_DEFAULT_PASS);
            // -- List updating
            listsLastUpdated = c->get(CONFIG__COUNTRYLOOKUP_LAST_UPDATED, 0LL);
            listsUpdatePeriod = c->get(CONFIG__COUNTRYLOOKUP_UPDATE_INTERVAL, COUNTRYLOOKUP_LIST_DEFAULT_UPDATE_INTERVAL);
            
            unique_lock<mutex> cl(mutexService);
            connection *conn;
            while(!shutdown)
            {
                // Check if the lists need updating
                if(Utils::getEpochTimeMs() > listsLastUpdated + listsUpdatePeriod)
                    updateLists();
                // Check if there are any IPs with unresolved country's
                try
                {
                    conn = createDbConnection();
                    // Check we have an open connection, else reset it
                    if(conn == 0 || !conn->is_open())
                    {
                        cerr << getTitle() << ": failed to connect to the database (null-pointer: " << (conn == 0 ? "yes" : "no") << ")!" << endl;
                        delete conn;
                        conn = 0;
                    }
                    else
                    {
                        // Check for IPs with unresolved country's
                        // -- The 'work' class is a transaction wrapper, thus we keep recreating it to
                        // -- avoid the possibility of locking any database resources since finding an IP
                        // -- may take a long time.
                        result ips;
                        {
                            work w(*conn);
                            ips = w.exec("SELECT ipid, ip FROM bc.tb_ip_lookup WHERE locationid='0';");
                        }
                        if(ips.size() > 0)
                        {
                            // Process each IP
                            string country;
                            for(result::const_iterator it = ips.begin(); it != ips.end(); it++)
                            {
                                bool found = findIPCountry(string(it["ip"].c_str()), country);
                                if(found)
                                    cout << getTitle() << ": result for ip '" << it["ip"].c_str() << "' - '" << country << "'!" << endl;
                                else
                                    cout << getTitle() << ": no country could be found for ip '" << it["ip"].c_str() << "'!" << endl;
                                // Update the database with the result
                                work w(*conn);
                                int locationid;
                                if(found)
                                {
                                    // Check if the location/country already exists, else create a new record
                                    result r = w.exec("SELECT locationid FROM bc.tb_locations WHERE name='" + escape_binary(country) + "'");
                                    if(r.size() > 0)
                                        locationid = r[0][0].as<int>();
                                    else
                                        locationid = w.exec("INSERT INTO bc.tb_locations (locationid, name) VALUES((SELECT MAX(locationid) FROM bc.tb_locations)+1, '" + escape_binary(country) + "'); SELECT MAX(locationid) FROM bc.tb_locations;")[0][0].as<int>();
                                }
                                else
                                    locationid = 1;
                                // Update entry for IP
                                w.exec("UPDATE bc.tb_ip_lookup SET locationid='" + escape_binary(Utils::intToString(locationid)) + "' WHERE ipid='" + escape_binary(it["ipid"].c_str()) + "';");
                                w.commit();
                            }
                        }
                        // Since we may block for a long duration, it's better to recreate the database connection each iteration.
                        delete conn;
                    }
                }
                catch(broken_connection ex)
                {
                    cerr << getTitle() << ": connection to the database server has been lost!" << endl;
                    if(conn != 0)
                        delete conn;
                    conn = createDbConnection();
                }
                catch(std::exception ex)
                {
                    cerr << getTitle() << ": unknown exception occurred whilst checking for country look-ups!" << endl;
                }
                // Wait for an event (block the thread - avoid intense CPU usage)...
                cvService.wait(cl);
            }
        }
        bool CountryLookup::findIPCountry(string ip, string &country)
        {
            cout << getTitle() << ": finding country for IP '" << ip << "'." << endl;
            IPAddress ipp(ip);
            // Iterate each file and parse each IP to test if the target IP belongs
            for(map<string, string>::iterator it = registrantLists.begin(); it != registrantLists.end(); it++)
            {
                ifstream file((*it).first);
                if(file.is_open())
                {
                    string line;
                    while(getline(file, line, '\n'))
                    {
                        vector<string> tokens = Utils::split(line, '|');
                        if(tokens.size() == 7 && tokens.at(2) == "ipv4" && (tokens.at(6) == "allocated" || tokens.at(6) == "assigned"))
                        {
                            int offset;
                            if(Utils::tryParse(tokens.at(4), offset))
                            {
                                IPAddress ipb_start(tokens.at(3));
                                IPAddress ipb_end = IPAddress::getAddressOffset(ipb_start, offset);
                                if(ipp >= ipb_start && ipp <= ipb_end)
                                {
                                    country = tokens.at(1);
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
            return false;
        }
        void CountryLookup::updateLists()
        {
            cout << getTitle() << ": updating lists..." << endl;
            string url;
            string file;
            stringstream ss;
            int bytes;
            for(map<string,string>::const_iterator it = registrantLists.begin(); it != registrantLists.end(); it++)
            {
                url = (*it).second;
                file = (*it).first;
                cout << getTitle() << ": updating list '" << url << "'..." << endl;
                int t = url.find_first_of('/');
                string host = url.substr(0, t);
                string path = url.substr(t+1);
                ClientSocket cs;
                bool connected;
                for(int i = 1; i <= 10 && !(connected = cs.connectToHost(host, 80)); i++)
                {
                    cerr << "Failed to connect to list '" << url << "', attempt #" << i << "!" << endl;
                    Utils::sleep(1000);
                }
                if(connected)
                {
                    ss << "GET /" << path << " HTTP/1.1\r\nHost: " << host << "\r\nUser-Agent: binaryclockv2" << "\r\n\r\n"; 
                    cs.write(ss, true);
                    ofstream os(file);
                    if(os.is_open())
                    {
                        bytes = cs.readIntoStream(os);
                        os.close();
                        cout << getTitle() << ": successfully updated list '" << url << "' - " << bytes << " bytes!" << endl;
                    }
                    else
                        cerr << getTitle() << ": failed to open file for list '" << url << "' at file '" << file << "'!" << endl;
                }
                else
                    cerr << "Failed to update list '" << url << "' - failed to connect!" << endl;
                Utils::sleep(1000);
            }
            cout << getTitle() << ": successfully finished updating IP block allocation lists!" << endl;
            // Update updated time
            listsLastUpdated = Utils::getEpochTimeMs();
            Configurator *c = static_cast<Configurator*>(controller->getServiceByName(SERVICETITLE_CONFIGURATOR));
            if(c != 0)
                c->set(CONFIG__COUNTRYLOOKUP_LAST_UPDATED, Utils::longlongToString(listsLastUpdated));
        }
        connection* CountryLookup::createDbConnection()
        {
            connection *conn = 0;
            try
            {
                conn = new connection(qs);
            }
            catch(std::exception ex)
            {
                
                if(conn != 0)
                {
                    delete conn;
                    conn = 0;
                }
            }
            return conn;
        }
    }
}