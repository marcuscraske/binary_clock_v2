#include "Pages.h"

#include <sstream>
#include <stdlib.h>

#include "Utils.h"
#include "Alarm.h"

using std::stringstream;

using BC::Services::Alarm;

namespace BC
{
    namespace Web
    {
        namespace Modules
        {
            bool Pages::moduleRegister(HttpHandler * const handler)
            {
                // Register functions
                handler->functionRegister(this, "pageAdmin", pageAdmin);
                handler->functionRegister(this, "pageGuestbook", pageGuestbook);
                handler->functionRegister(this, "pageToolsSystemInfo", pageTools_SystemInfo);
                handler->functionRegister(this, "pageToolsAPI", pageTools_API);
                handler->functionRegister(this, "pageToolsRandomPornVideo", pageTools_RandomPornVideo);
                handler->functionRegister(this, "pageToolRandomGenerator", pageTools_RandomGenerator);
                // Build database connection string
                dbConnectionString =     "hostaddr=" + handler->getService_Configurator()->get(CONFIG__DATABASE_HOST, MODULES_PAGES_DEFAULT_HOST) +
                                                " port=" + Utils::intToString(handler->getService_Configurator()->get(CONFIG__DATABASE_PORT, MODULES_PAGES_DEFAULT_PORT)) +
                                                " dbname=" + handler->getService_Configurator()->get(CONFIG__DATABASE_DB, MODULES_PAGES_DEFAULT_DATABASE) +
                                                " user=" + handler->getService_Configurator()->get(CONFIG__DATABASE_USER, MODULES_PAGES_DEFAULT_USER) +
                                                " password=" + handler->getService_Configurator()->get(CONFIG__DATABASE_PASS, MODULES_PAGES_DEFAULT_PASS);
                guestbookThreshold = handler->getService_Configurator()->get(CONFIG__GUESTBOOK_POST_THRESHOLD, MODULES_PAGES_GUESTBOOK_DEFAULT_POST_THRESHOLD);
                return true;
            }
            bool Pages::isLocalNetwork(char *ip)
            {
                string filter = string(MODULES_PAGES_DEFAULT_ADMIN_IP_START);
                string ips = string(ip);
                return ips == "127.0.0.1" || (filter.length() < ips.length() && ips.substr(0, filter.length()) == filter);
            }
            string Pages::pageAdmin(HttpHandler * const handler, IModule *module, Client *client, HttpRequest *request, HttpResponse *response, string arguments, string &pageUpper, string &pageLower)
            {
                // Check the client is a local IP
                if(!isLocalNetwork(client->ipAddress))
                    // Show access-denied page
                    return handler->getDiskCache()->fetch("admin_denied.bct", "Failed to load admin_denied.bct; access denied!");
                // Get the sub-page from the query-string
                request->parseQueryString();
                string page = request->queryStrings["page"];
                string content;
                if(page.length() == 0 || page == "home")
                    // Admin home-page
                    content = handler->getDiskCache()->fetch("admin_home.bct", "Failed to load admin_home.bct!");
                else if(page == "alarm")
                {
                    // Alarm-clock
                    // -- Check for deletion of event ('yyyymmddhhmmss' - 14 chars)
                    if(request->queryStrings["delete"].length() == 14)
                    {
                        string raw = request->queryStrings["delete"];
                        int year, month, day, hour, minute, second;
                        if(     Utils::tryParse(raw.substr(0, 4), year) &&
                                Utils::tryParse(raw.substr(4, 2), month) &&
                                Utils::tryParse(raw.substr(6, 2), day) &&
                                Utils::tryParse(raw.substr(8, 2), hour) &&
                                Utils::tryParse(raw.substr(10, 2), minute) &&
                                Utils::tryParse(raw.substr(12, 2), second)
                        )
                        {
                            DateTime at = DateTime(year, month, day, hour, minute, second);
                            handler->getService_Alarm()->remove(at);
                            response->redirect("/admin?page=alarm");
                        }
                    }
                    // -- Check for addition of event
                    string  year = request->formData["year"],
                            month = request->formData["month"],
                            day = request->formData["day"],
                            hour = request->formData["hour"],
                            minute = request->formData["minute"],
                            second = request->formData["second"];
                    string error;
                    if(year.length() > 0 && month.length() > 0 && day.length() > 0 && hour.length() > 0 && minute.length() > 0 && second.length() > 0)
                    {
                        // Validate input
                        int pYear, pMonth, pDay, pHour, pMinute, pSecond;
                        DateTime time = Utils::getDateTime();
                        // Add and save config
                        if(!Utils::tryParse(year, pYear) || pYear < time.year)
                            error = "Invalid year!";
                        else if(!Utils::tryParse(month, pMonth))
                            error = "Invalid month!";
                        else if(!Utils::tryParse(day, pDay))
                            error = "Invalid year!";
                        else if(!Utils::tryParse(hour, pHour))
                            error = "Invalid hour!";
                        else if(!Utils::tryParse(minute, pMinute))
                            error = "Invalid minute!";
                        else if(!Utils::tryParse(second, pSecond))
                            error = "Invalid second!";
                        else
                        {
                            // Check the date has not already passed i.e. would set the alarm-off straight away
                            DateTime alarmNew = DateTime(pYear, pMonth, pDay, pHour, pMinute, pSecond);
                            DateTime alarmCurr = DateTime(time.year, time.month, time.day, time.hour, time.minute, time.second);
                            if(alarmCurr > alarmNew)
                                error = "The specified time has already passed!";
                            else
                            {
                                handler->getService_Alarm()->add(alarmNew);
                                content = handler->getDiskCache()->fetch("admin_alarm_success.bct", "Failed to load alarm success template!");
                            }
                        }
                    }
                    if(content.length() == 0)
                    {
                        content = handler->getDiskCache()->fetch("admin_alarm.bct", "Failed to load alarm template!");
                        // Check if to display an error-message
                        if(error.length() > 0)
                        {
                            Utils::replace(content, "%ERROR%", error);
                            Utils::replace(content, "%ERROR_CLASS%", "VISIBLE");
                        }
                    }
                }
                else if(page == "settings")
                {
                    // Settings
                    Configurator *config = handler->getService_Configurator();
                    if(config == 0)
                        content = "Configurator service is not running!";
                    else
                    {
                        // -- Check for post-back of any settings
                        if(request->formData.size() > 0)
                        {
                            // -- Save keys to configurator service
                            for(map<string, string>::iterator it = request->formData.begin(); it != request->formData.end(); it++)
                                    config->set((*it).first, Utils::urlDecodeBasic((*it).second));
                            // Restart all services
                            handler->getService_WebHttp()->getController()->restartServices();
                        }
                        // -- Display each setting
                        string templateSettings;
                        string templateSettingsSection;
                        string templateItemText;
                        string templateItemSequence;
                        // -- -- Load Templates
                        if(!handler->getDiskCache()->fetchLoad("admin_settings.bct", templateSettings))
                            content = "Failed to load settings template!";
                        else if(!handler->getDiskCache()->fetchLoad("admin_settings_section.bct", templateSettingsSection))
                            content = "Failed to load settings section template!";
                        else if(!handler->getDiskCache()->fetchLoad("admin_settings_text.bct", templateItemText))
                            content = "Failed to load settings text template!";
                        else if(!handler->getDiskCache()->fetchLoad("admin_settings_sequence.bct", templateItemSequence))
                            content = "Failed to load settings sequence template!";
                        else
                        {
                            // -- -- Build output
                            stringstream output;
                            {
                                stringstream section;
                                map<string, string> kv = config->getAll();
                                string sectionPrev, sectionCurr;
                                string k, v, i; // k = key, v = value, i = temp for item replacement
                                for(map<string, string>::iterator it = kv.begin(); it != kv.end(); it++)
                                {
                                    k = (*it).first;
                                    v = (*it).second;
                                    // Decide if we're in a new section
                                    int sep = k.find_first_of('.');
                                    if(sep == string::npos || sep == k.length())
                                        sectionCurr = "";
                                    sectionCurr = sep == string::npos || sep == k.length() ? "" : k.substr(0, sep);
                                    // Dump previous section if it contains anythings
                                    if(sectionCurr != sectionPrev)
                                    {
                                        string prev = section.str();
                                        if(prev.length() > 0)
                                        {
                                            if(sectionPrev.length() > 0)
                                            {
                                                // Output the section
                                                string t = templateSettingsSection;
                                                Utils::replace(t, "%TITLE%", sectionPrev);
                                                Utils::replace(t, "%ITEMS%", prev);
                                                output << t;
                                            }
                                            else
                                                output << prev;
                                            // Clear the section buffer, ready for the new section
                                            section.str(std::string());
                                            section.clear();
                                        }
                                        sectionPrev = sectionCurr;
                                    }
                                    // Write the current item
                                    if(sectionCurr == "display" && (k == CONFIG__DISPLAY__LIGHT_LOW || k == CONFIG__DISPLAY__LIGHT_HIGH || k == CONFIG__DISPLAY__LIGHT_BUZZ))
                                    {
                                        i = templateItemSequence;
                                        // Generate possible sequence selections
                                        stringstream seqs;
                                        string title;
                                        int vI = -1;
                                        Utils::tryParse(v, vI);
                                        for(int i = 0; i < Display::NUMBER_OF_SEQUENCES; i++)
                                        {
                                            // Get title
                                            switch(i)
                                            {
                                                case Display::Sequence::AllLeds:
                                                    title = "All On";
                                                    break;
                                                case Display::Sequence::Failure:
                                                    title = "Error - Failure";
                                                    break;
                                                case Display::Sequence::ManualBuffer:
                                                    title = "Manual via Buffer";
                                                    break;
                                                case Display::Sequence::Offline:
                                                    title = "All Off";
                                                    break;
                                                case Display::Sequence::SingleLedTest:
                                                    title = "Test - Single LED Increment";
                                                    break;
                                                case Display::Sequence::SymbolX:
                                                    title = "Symbol - X";
                                                    break;
                                                case Display::Sequence::Time:
                                                    title = "Binary Time";
                                                    break;
                                                default:
                                                    title = "Unknown Sequence";
                                                    break;
                                            }
                                            // Generate select item
                                            if(i == vI)
                                                seqs << "<option value=\"" << i << "\" selected>" << title << "</option>";
                                            else
                                                seqs << "<option value=\"" << i << "\">" << title << "</option>";
                                        }
                                        // Replace template
                                        Utils::replace(i, "%TITLE%", k);
                                        Utils::replace(i, "%ITEMS%", seqs.str());
                                    }
                                    else
                                    {
                                        i = templateItemText;
                                        Utils::replace(i, "%TITLE%", k);
                                        Utils::replace(i, "%VALUE%", v);
                                    }
                                    // Append item data
                                    section << i;
                                }
                                // Write last section
                                string prev = section.str();
                                if(prev.length() > 0)
                                {
                                    if(sectionCurr.length() > 0)
                                    {
                                        string t = templateSettingsSection;
                                        Utils::replace(t, "%TITLE%", sectionCurr);
                                        Utils::replace(t, "%ITEMS%", prev);
                                        output << t;
                                    }
                                    else
                                        output << prev;
                                }
                            }
                            Utils::replace(templateSettings, "%SETTINGS%", output.str());
                            content = templateSettings;
                        }
                    }
                }
                else if(page == "services")
                {
                    // Service control page
                    string action = request->queryStrings["action"];
                    string service = request->queryStrings["service"];
                    if(action.length() != 0 && service.length() != 0)
                    {
                        // Get the service
                        Utils::replace(service, "_", " ");
                        IService *s = handler->getService_WebHttp()->getController()->getServiceByName(service);
                        // Apply action
                        if(s != 0)
                        {
                            if(action == "start")
                                s->start();
                            if(action == "stop")
                                s->stop();
                            if(action == "restart")
                            {
                                s->stop();
                                s->start();
                            }
                        }
                    }
                    if(!handler->getDiskCache()->fetchLoad("admin_services.bct", content))
                        content = "Failed to load services template!";
                }
                else if(page == "reload_cache")
                {
                    if(!handler->getDiskCache()->fetchLoad("admin_cache_reloaded.bct", content))
                        content = "Failed to load cache reloaded template!";
                    handler->getDiskCache()->reload(handler->getService_WebHttp()->getController());
                }
                else if(page == "disk_cache")
                {
                    content = handler->getDiskCache()->fetch("admin_cache.bct", "Failed to load disk cache layout template!");
                    string templateItem = handler->getDiskCache()->fetch("admin_cache_item.bct", "Failed to load disk cache item template!");
                    // Display list of items
                    stringstream items;
                    unique_lock<mutex> lockCaache(*handler->getDiskCache()->getMutex());
                    map<string, DiskCacheItem*> const *its = handler->getDiskCache()->getItems();
                    string item;
                    long long totalSize = 0LL;
                    for(map<string, DiskCacheItem*>::const_iterator it = its->begin(); it != its->end(); it++)
                    {
                        item = templateItem;
                        Utils::replace(item, "%KEY%", (*it).first);
                        Utils::replace(item, "%SIZE%", Utils::bytesToHumanString((*it).second->getLength()));
                        totalSize += (*it).second->getLength();
                        items << item;
                    }
                    Utils::replace(content, "%BASE_PATH%", handler->getDiskCache()->getBasePath());
                    Utils::replace(content, "%TOTAL_SIZE%", Utils::bytesToHumanString(totalSize));
                    Utils::replace(content, "%ITEMS%", items.str());
                }
                else if(page == "country_lookup_wake")
                {
                    if(!handler->getDiskCache()->fetchLoad("admin_country_wakeup.bct", content))
                        content = "Failed to load IP country lookup service wake-up template!";
                    handler->getCountryLookup()->wake();
                }
                else if(page == "terminate")
                {
                    if(!handler->getDiskCache()->fetchLoad("admin_terminated.bct", content))
                        content = "Failed to load services template!";
                    handler->getService_WebHttp()->getController()->requestShutdown();
                }
                else if(page == "shutdown")
                {
                    if(request->queryStrings["confirm"].length() != 0)
                    {
                      system("halt");
                      if(!handler->getDiskCache()->fetchLoad("admin_shutdown_success.bct", content))
                        content = "Failed to load shutdown success template!";
                    }
                    else if(!handler->getDiskCache()->fetchLoad("admin_shutdown.bct", content))
                        content = "Failed to load shutdown confirm template!";
                }
                else if(page == "reboot")
                {
                    if(request->queryStrings["confirm"].length() != 0)
                    {
                        system("reboot");
                        if(!handler->getDiskCache()->fetchLoad("admin_reboot_success.bct", content))
                            content = "Failed to load reboot success template!";
                    }
                    else if(!handler->getDiskCache()->fetchLoad("admin_reboot.bct", content))
                        content = "Failed to load reboot confirm template!";
                }
                // Format the page
                content = Templates::format(handler, content, client, request, response);
                return content;
            }
            string Pages::pageGuestbook(HttpHandler * const handler, IModule *module, Client *client, HttpRequest *request, HttpResponse *response, string arguments, string &pageUpper, string &pageLower)
            {
                Pages *m = static_cast<Pages*>(module);
                // Create a connection to the database
                try
                {
                    connection conn(m->dbConnectionString);
                    // Check we have an open connection
                    if(!conn.is_open())
                        return handler->getDiskCache()->fetch("database_failure.bct", "Failed to load database failure template!");
                    // Check the user is not banned
                    {
                        work banCheck(conn);
                        result banCheckR = banCheck.exec("SELECT COUNT('') FROM bc.tb_ipbans WHERE ip='" + escape_binary(client->ipAddress) + "'");
                        if(banCheckR.size() == 1 && banCheckR[0].at(0).as<int>() > 0)
                            return handler->getDiskCache()->fetch("guestbook_banned.bct", "Failed to load guest-book banned page!");
                    }
                    stringstream output;
                    // Field Data
                    string message = Utils::urlDecodeBasic(request->formData["gb_message"]);
                    string name = Utils::urlDecodeBasic(request->formData["gb_name"]);
                    string parent = Utils::urlDecodeBasic(request->formData["gb_parent"]);
                    string error = "";
                    request->parseQueryString();
                    if(parent.length() == 0 && request->queryStrings["reply"].length() > 0)
                        parent = request->queryStrings["reply"];
                    // Check if the user has attempted to make a post
                    if(message.length() > 0 && name.length() > 0)
                    {
                        int parentPostId = 0;
                        // Validate the data
                        if(message.length() < MODULES_PAGES_GUESTBOOK_MESSAGE_MIN || message.length() > MODULES_PAGES_GUESTBOOK_MESSAGE_MAX)
                            error = "Message must be " + Utils::intToString(MODULES_PAGES_GUESTBOOK_MESSAGE_MIN) + " to " + Utils::intToString(MODULES_PAGES_GUESTBOOK_MESSAGE_MAX) + " characters in length!";
                        else if(name.length() < MODULES_PAGES_GUESTBOOK_NAME_MIN || name.length() > MODULES_PAGES_GUESTBOOK_NAME_MAX)
                            error = "Name must be " + Utils::intToString(MODULES_PAGES_GUESTBOOK_NAME_MIN) + " to " + Utils::intToString(MODULES_PAGES_GUESTBOOK_NAME_MAX) + " characters in length!";
                        else if(parent.length() > 0 && !Utils::tryParse(parent, parentPostId))
                            error = "Invalid parent post ID!";
                        else
                        {
                            work np(conn);
                            // Fetch the IP for the current user, else create a record
                            result ipidR = np.exec("SELECT ipid FROM bc.tb_ip_lookup WHERE ip='" + escape_binary(client->ipAddress) + "'");
                            int ipid;
                            bool newIp = false;
                            if(ipidR.size() != 0)
                                ipid = ipidR[0][0].as<int>();
                            else
                            {
                                ipid = np.exec("INSERT INTO bc.tb_ip_lookup (ipid, ip) VALUES(COALESCE((SELECT MAX(ipid) FROM bc.tb_ip_lookup), 1)+1, '" + escape_binary(client->ipAddress) + "'); SELECT MAX(ipid) FROM bc.tb_ip_lookup;")[0][0].as<int>();
                                // Inform the IP lookup service
                                newIp = true;
                            }
                            // Check the user has not made too many posts in the last hour
                            result postC = np.exec("SELECT COUNT('') FROM bc.tb_guestbook WHERE ipid='" + escape_binary(Utils::intToString(ipid)) + "'");
                            if(postC.size() > 0 && postC[0][0].as<int>() >= m->guestbookThreshold)
                                error = "You have made too many posts within the last hour; calm down or you will be banned!";
                            else
                            {
                                // Create the guestbook post
                                try
                                {
                                    np.exec("INSERT INTO bc.tb_guestbook (postid, parent_postid, ipid, name, message, datetime) VALUES(COALESCE((SELECT MAX(postid)+1 FROM bc.tb_guestbook), 1), " + (parentPostId == 0 ? "NULL" : "'" + Utils::intToString(parentPostId) + "'") + ", '" + Utils::intToString(ipid) + "', '" + escape_binary(name) + "', '" + escape_binary(message) + "', current_timestamp);");
                                    np.commit();
                                    if(newIp)
                                        handler->getCountryLookup()->wake();
                                    // Success - reset field data
                                    name = message = parent = "";
                                }
                                catch(integrity_constraint_violation ex)
                                {
                                    error = "Parent post does not exist!";
                                    np.abort();
                                }
                            }
                        }
                    }
                    // Append postbox to the top of the page
                    {
                        string postbox = handler->getDiskCache()->fetch("guestbook_postbox.bct", "Failed to load guestbook postbox template!");
                        Utils::replace(postbox, "%NAME%", name.length() > 0 ? Utils::htmlEncode(request->formData["gb_name"]) : "");
                        Utils::replace(postbox, "%PARENT%", parent.length() > 0 ? Utils::htmlEncode(request->formData["gb_parent"]) : "");
                        Utils::replace(postbox, "%MESSAGE%", message.length() > 0 ? Utils::htmlEncode(request->formData["gb_message"]) : "");
                        Utils::replace(postbox, "%MESSAGE_MAX%", Utils::intToString(MODULES_PAGES_GUESTBOOK_MESSAGE_MAX));
                        output << postbox;
                    }
                    // Check for admin options
                    bool islocal = isLocalNetwork(client->ipAddress); // Used for special rights to e.g. delete posts, ban users etc.
                    if(islocal)
                    {
                        string option = request->queryStrings["option"];
                        string pid = request->queryStrings["pid"];
                        if(option.length() > 0 && pid.length() > 0)
                        {
                            work w(conn);
                            // Fetch the post data
                            result postR = w.exec("SELECT * FROM bc.vi_guestbook_posts WHERE postid='" + escape_binary(pid) + "'");
                            // Apply option to post/author
                            if(option == "ban")
                            {
                                w.exec("INSERT INTO bc.tb_ipbans (banid, ip) VALUES(COALESCE((SELECT MAX(banid) FROM bc.tb_ipbans), 0)+1, '" + escape_binary(postR[0]["ip"].c_str()) + "'); DELETE FROM bc.tb_guestbook WHERE ipid='" + escape_binary(postR[0]["ipid"].c_str()) + "';");
                                w.commit();
                            }
                            else if(option == "delete")
                            {
                                w.exec("DELETE FROM bc.tb_guestbook WHERE postid='" + escape_binary(postR[0]["postid"].c_str()) + "'");
                                w.commit();
                            }
                            else if(option == "delete_all")
                            {
                                w.exec("DELETE FROM bc.tb_guestbook WHERE ipid='" + escape_binary(postR[0]["ipid"].c_str()) + "'");
                                w.commit();
                            }
                            else
                                error = "Unknown admin option '" + option + "'!";
                        }
                    }
                    // Append error-box
                    if(error.length() > 0)
                    {
                        string errorbox = handler->getDiskCache()->fetch("error_box.bct", "Failed to load error-box template!");
                        Utils::replace(errorbox, "%ERROR%", error);
                        output << errorbox;
                    }
                    // Get the page of posts to display
                    int page = 1;
                    Utils::tryParse(request->queryStrings["page"], page);
                    if(page < 1) page = 1;
                    // Fetch the posts
                    string templatePost = handler->getDiskCache()->fetch(islocal ? "guestbook_post_admin.bct" : "guestbook_post.bct", "Failed to load post template!");
                    string templateContainer = handler->getDiskCache()->fetch("guestbook_thread_container.bct", "Failed to load thread container template!");
                    work *posts = new work(conn);
                    result postsR = posts->exec("SELECT * FROM bc.vi_guestbook_posts WHERE parent_postid IS NULL LIMIT " + Utils::intToString(MODULES_PAGES_GUESTBOOK_POSTSPERPAGE) + " OFFSET " + Utils::intToString((MODULES_PAGES_GUESTBOOK_POSTSPERPAGE * page) - MODULES_PAGES_GUESTBOOK_POSTSPERPAGE));
                    if(postsR.size() != 0)
                        for(result::const_iterator it = postsR.begin(); it != postsR.end(); it++)
                            output << pageGuestbook_recursiveRenderSubPosts(it, templateContainer, templatePost, posts);
                    else
                        output << handler->getDiskCache()->fetch("guestbook_noposts.bct", "Failed to load guestbook no posts template!");
                    // Append page navigator
                    string nav = handler->getDiskCache()->fetch("guestbook_page_nav.bct", "Failed to load guestbook page navigator!");
                    Utils::replace(nav, "%PAGE%", Utils::intToString(page));
                    Utils::replace(nav, "%PREV%", Utils::intToString(page > 1 ? page-1 : 1));
                    Utils::replace(nav, "%NEXT%", Utils::intToString(page < INT32_MAX ? page+1 : INT32_MAX));
                    Utils::replace(nav, "%VISIBLE_LEFT%", page > 1 ? "VISIBLE" : "");
                    Utils::replace(nav, "%VISIBLE_RIGHT%", page < INT32_MAX && postsR.size() == MODULES_PAGES_GUESTBOOK_POSTSPERPAGE ? "VISIBLE" : "");
                    output << nav;
                    delete posts;
                    
                    return output.str();
                }
                catch(std::exception)
                {
                    return handler->getDiskCache()->fetch("database_failure.bct", "Failed to load database failure template!");
                }
            }
            string Pages::pageGuestbook_renderPost(result::const_iterator tuple, string &templatePost)
            {
                string post = templatePost;
                Utils::replace(post, "%POSTID%", tuple["postid"].c_str());
                Utils::replace(post, "%MESSAGE%", Utils::htmlEncode(tuple["message"].c_str()));
                Utils::replace(post, "%NAME%", Utils::htmlEncode(tuple["name"].c_str()));
                Utils::replace(post, "%DATETIME%", tuple["datetime"].c_str());
                Utils::replace(post, "%DATETIME_RAW%", tuple["datetime_raw"].c_str());
                Utils::replace(post, "%DATETIME_FORMATTED%", tuple["datetime_formatted"].c_str());
                Utils::replace(post, "%COUNTRY_NAME%", tuple["cname"].c_str());
                Utils::replace(post, "%COUNTRY_FLAG%", tuple["flag_img"].c_str());
                Utils::replace(post, "%IP%", tuple["ip"].c_str());
                return post;
            }
            string Pages::pageGuestbook_recursiveRenderSubPosts(result::const_iterator postTuple, string &templateContainer, string &templatePost, work *w)
            {
                // Check there are multiple posts, else just return the rendered parent post
                if(postTuple["subposts"].as<int>() == 0)
                    return pageGuestbook_renderPost(postTuple, templatePost);
                // Fetch and render children posts
                result cposts = w->exec("SELECT * FROM bc.vi_guestbook_posts WHERE parent_postid='" + escape_binary(postTuple["postid"].c_str()) +"' ORDER BY datetime ASC");
                stringstream output;
                for(result::const_iterator it = cposts.begin(); it != cposts.end(); it++)
                    output << pageGuestbook_recursiveRenderSubPosts(it, templateContainer, templatePost, w);
                // Return rendered parent post along with child posts inside a container
                string container = templateContainer;
                Utils::replace(container, "%SUB_POSTS%", output.str());
                return pageGuestbook_renderPost(postTuple, templatePost) + container;
            }
            string Pages::pageTools_SystemInfo(HttpHandler * const handler, IModule *module, Client *client, HttpRequest *request, HttpResponse *response, string arguments, string &pageUpper, string &pageLower)
            {
                string content;
                if(!handler->getDiskCache()->fetchLoad("tools/system_info_layout.bct", content))
                    content = "Failed to load system information layout template!";
                else
                {
                    SystemInfo si;
                    // Output CPU usage
                    Utils::replace(content, "%CPU_USAGE_ALL%", Utils::doubleToString(si.cpuUsageAll));
                    Utils::replace(content, "%CPU_USAGE_USER%", Utils::doubleToString(si.cpuUsageUser));
                    Utils::replace(content, "%CPU_USAGE_NICE%", Utils::doubleToString(si.cpuUsageNice));
                    Utils::replace(content, "%CPU_USAGE_SYSTEM%", Utils::doubleToString(si.cpuUsageSystem));
                    Utils::replace(content, "%CPU_USAGE_IDLE%", Utils::doubleToString(si.cpuUsageIdle));
                    // Output memory usage
                    Utils::replace(content, "%MEMORY_TOTAL%", Utils::bytesToHumanString(si.memoryTotal));
                    Utils::replace(content, "%MEMORY_FREE%", Utils::bytesToHumanString(si.memoryFree));
                    Utils::replace(content, "%MEMORY_SHARED%", Utils::bytesToHumanString(si.memoryShared));
                    Utils::replace(content, "%MEMORY_USAGE%", Utils::bytesToHumanString(si.memorySelfSize));
                    Utils::replace(content, "%MEMORY_PEAK%", Utils::bytesToHumanString(si.memorySelfPeak));
                    // Output misc info
                    Utils::replace(content, "%MISC_PROCS%", Utils::intToString(si.miscProcs));
                    Utils::replace(content, "%MISC_UPTIME%", Utils::longToString(si.miscUptime));
                    Utils::replace(content, "%MISC_SELF_UPTIME%", Utils::getSecondsAsString((Utils::getEpochTimeMs() - handler->getEpochStartTimeMs()) / 1000LL));
                    Utils::replace(content, "%MISC_TIME%", std::to_string(si.miscTime.day) + "/" + std::to_string(si.miscTime.month) + "/" + std::to_string(si.miscTime.year) + " " + (si.miscTime.hour < 10 ? "0" : "") + std::to_string(si.miscTime.hour) + ":" + (si.miscTime.minute < 10 ? "0" : "") + std::to_string(si.miscTime.minute) + ":" + (si.miscTime.second < 10 ? "0" : "") + std::to_string(si.miscTime.second));
                    Utils::replace(content, "%MISC_PID%", std::to_string(si.miscPid));
                }
                return content;
            }
            string Pages::pageTools_API(HttpHandler * const handler, IModule *module, Client *client, HttpRequest *request, HttpResponse *response, string arguments, string &pageUpper, string &pageLower)
            {
                stringstream output;
                output << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
                output << "<!--Binary Clock V2 Read-Only API-->" << endl;
                output << "<!--The purpose of this API is for simple read-only status indication and general information.-->" << endl;
                output << "<api>" << endl;
                // Sensor data
                output << "     <sensors>" << endl;
                output << "             <temperature_celsius>" << handler->getService_Sensors()->getTemperature() << "</temperature_celsius>" << endl;
                output << "             <light_percent>" << handler->getService_Sensors()->getLight() << "</light_percent>" << endl;
                output << "     </sensors>" << endl;
                // System info
                SystemInfo si;
                output << "     <system_info>" << endl;
                output << "             <cpu>" << endl;
                output << "                     <all>" << si.cpuUsageAll << "</all>" << endl;
                output << "                     <user>" << si.cpuUsageUser << "</user>" << endl;
                output << "                     <nice>" << si.cpuUsageNice << "</nice>" << endl;
                output << "                     <system>" << si.cpuUsageSystem << "</system>" << endl;
                output << "                     <idle>" << si.cpuUsageIdle << "</idle>" << endl;
                output << "             </cpu>" << endl;
                output << "             <memory>" << endl;
                output << "                     <total>" << si.memoryTotal << "</total>" << endl;
                output << "                     <free>" << si.memoryFree << "</free>" << endl;
                output << "                     <shared>" << si.memoryShared << "</shared>" << endl;
                output << "                     <self_size>" << si.memorySelfSize << "</self_size>" << endl;
                output << "                     <self_peak>" << si.memorySelfPeak << "</self_peak>" << endl;
                output << "             </memory>" << endl;
                output << "             <misc>" << endl;
                output << "                     <processes>" << si.miscProcs << "</processes>" << endl;
                output << "                     <pid>" << si.miscPid << "</pid>" << endl;
                output << "                     <time>" << endl;
                output << "                             <day>" << si.miscTime.day << "</day>" << endl;
                output << "                             <month>" << si.miscTime.month << "</month>" << endl;
                output << "                             <year>" << si.miscTime.year << "</year>" << endl;
                output << "                             <hour>" << si.miscTime.hour << "</hour>" << endl;
                output << "                             <minute>" << si.miscTime.minute << "</minute>" << endl;
                output << "                             <second>" << si.miscTime.second << "</second>" << endl;
                output << "                     </time>" << endl;
                output << "                     <uptime_seconds>" << si.miscUptime << "</uptime_seconds>" << endl;
                output << "                     <self_uptime_seconds>" << (Utils::getEpochTimeMs() - handler->getEpochStartTimeMs()) << "</self_uptime_seconds>" << endl;
                output << "             </misc>" << endl;
                output << "     </system_info>" << endl;
                // Service info
                output << "     <services>" << endl;
                map<string, IService*> sv = handler->getService_WebHttp()->getController()->getServices();
                IService *se;
                int c = 0;
                for(map<string, IService*>::const_iterator s = sv.begin(); s != sv.end(); s++)
                {
                    se = (*s).second;
                    output << "         <service_" << c << ">" << endl;
                    output << "                 <title>" << se->getTitle() << "</title>" << endl;
                    output << "                 <running>" << (se->isRunning() ? "1" : "0") << "</running>" << endl;
                    output << "         </service_" << c++ << ">";
                }
                output << "     </services>" << endl;
                output << "</api>";
                response->contentType = "application/xml";
                return output.str();
            }
            string Pages::pageTools_RandomPornVideo(HttpHandler * const handler, IModule *module, Client *client, HttpRequest *request, HttpResponse *response, string arguments, string &pageUpper, string &pageLower)
            {
#define PAGES_RANDOMPORN_MOTHERLESS             "http://motherless.com/random/video"
#define PAGES_RANDOMPORN_REDTUBE                "http://www.redtube.com/"
#define PAGES_RANDOMPORN_REDTUBE_ID_MIN         1000
#define PAGES_RANDOMPORN_REDTUBE_ID_MAX         99999
#define PAGES_RANDOMPORN_YOUPORN                "http://www.youporn.com/watch/"
#define PAGES_RANDOMPORN_YOUPORN_ID_MIN         500
#define PAGES_RANDOMPORN_YOUPORN_ID_MAX         500000
#define PAGES_RANDOMPORN_YOUJIZZ                "http://www.youjizz.com/random.php"
#define PAGES_RANDOMPORN_HORNYBUNNY             "http://hornbunny.com/random/"
                // Setup random to use a random seed
                Utils::randomSetSeed();
                // First randomly pick a provider
                int provider = Utils::random(1, 5);
                // Handle provider
                string destURL = "";
                switch(provider)
                {
                    case 1:     // Motherless
                        destURL = PAGES_RANDOMPORN_MOTHERLESS;
                        break;
                    case 2:     // Redtube
                        destURL = PAGES_RANDOMPORN_REDTUBE + std::to_string(Utils::random(PAGES_RANDOMPORN_REDTUBE_ID_MIN, PAGES_RANDOMPORN_REDTUBE_ID_MAX));
                        break;
                    case 3:     // YouPorn
                        destURL = PAGES_RANDOMPORN_YOUPORN + std::to_string(Utils::random(PAGES_RANDOMPORN_YOUPORN_ID_MIN, PAGES_RANDOMPORN_YOUPORN_ID_MAX));
                        break;
                    case 4:     // YouJizz
                        destURL = PAGES_RANDOMPORN_YOUJIZZ;
                        break;
                    case 5:     // HornyBunny
                        destURL = PAGES_RANDOMPORN_HORNYBUNNY;
                        break;
                }
                response->statusCode = 307; // Temp. redirect
                response->headers["Location"] = destURL;
                response->headers["Cache-Control"] = "no-cache";
                return "<html><body><a href=\"" + destURL + "\">" + destURL + "</a></body></html>";
            }
            string Pages::pageTools_RandomGenerator(HttpHandler * const handler, IModule *module, Client *client, HttpRequest *request, HttpResponse *response, string arguments, string &pageUpper, string &pageLower)
            {
                // Load the layout template
                string content;
                if(!handler->getDiskCache()->fetchLoad("tools/random_number_layout.bct", content))
                    return "Failed to load random generator layout template!";
                string result = "";
                string min = request->formData["r_min"];
                string max = request->formData["r_max"];
                // Check for postback
                if(min.length() > 0 && max.length() > 0)
                {
                    int iMin, iMax;
                    if(!Utils::tryParse(min, iMin))
                        result = "Invalid minimum number!";
                    else if(!Utils::tryParse(max, iMax))
                        result = "Invalid maximum number!";
                    else if(min >= max)
                        result = "Minimum cannot be greater than maximum or be the same value.";
                    else
                    {
                        Utils::randomSetSeed();
                        result = std::to_string(Utils::random(iMin, iMax)) + " <br /><br />" + Utils::getTimeDateGBString();
                    }
                }
                Utils::replace(content, "%MINIMUM%", Utils::htmlEncode(min));
                Utils::replace(content, "%MAXIMUM%", Utils::htmlEncode(max));
                Utils::replace(content, "%RESULT%", Utils::htmlEncode(result));
                return content;
            }
        }
    }
}