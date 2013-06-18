
#include <stdint.h>

#include "Configurator.h"

using BC::Services::Configurator;

namespace BC
{
    namespace Services
    {
        void Configurator::prerun()
        {
            unique_lock<mutex> lock(mutexThread);
            load();
        }
        void Configurator::run()
        {
            // Every <time period>, we check if the configuration has been
            // altered; if so, we shall save it (unsavedConfig flag). This is
            // because I/O operations are expensive and we want to avoid
            // disk thrashing!
            unique_lock<mutex> cl(mutexService);
            while(!shutdown)
            {
                unique_lock<mutex> lock(mutexThread);
                // Check if the configuration is unsaved
                long long currTime = Utils::getEpochTimeMs();
                if(unsavedConfig && (currTime >= saveTime || currTime >= lastSave + CONFIG_SAVE_MAXWAIT) && save())
                    unsavedConfig = false;
                lock.unlock();
                // Either block the thread (if there's no config save pending) or sleep
                if(unsavedConfig)
                {
                    currTime = Utils::getEpochTimeMs(); // Update the time in-case save() was delayed e.g. slow I/O or thread block for a period of time
                    long long t1 = saveTime - currTime;
                    long long t2 = (lastSave + CONFIG_SAVE_MAXWAIT) - currTime;
                    if(t1 < 0 || t2 < 0) // Protection against dangerous 1 < 0 || t2 < 0 || t1 > INT64_MAX || t2 > INT64_MAX) // Protection against dangsleeping; although this should never occur unless save() took a long time.
                        cerr << getTitle() << ": warning (time error) - nearly attempted to sleep for period of '" << t1 << "' or '" << t2 << "' m/s! Skipping loop..." << endl;
                    else
                        Utils::conditionVariableSleep(cl, cvService, t1 < t2 ? t1 : t2);
                }
                else
                    cvService.wait(cl);
            }
            // Save settings
            cout << "Shutdown initiated, saving configuration..." << endl;
            save();
        }
        void Configurator::load()
        {
            // Load the settings from file
            ifstream file(CONFIG_FILE);
            if(file.is_open())
            {
                // Clear the existing keys
                config.clear();
                string line, key, value;
                int separator;
                while(file.good())
                {
                    getline(file, line, '\n');
                    line = Utils::trim(line);
                    separator = line.find('=');
                    if(!(line[0] == '#' || (line[0] == '-' && line[1] == '-') || (line[0] == '/' && line[1] == '/')) && separator != string::npos && separator <= line.length())
                    {
                        key = line.substr(0, separator);
                        value = separator == line.length() ? std::string() : Utils::trim(line.substr(separator+1));
                        std::replace(value.begin(), value.end(), CONFIG_NEWLINE_CHAR, '\n');
                        config[key] = value;
                        cout << getTitle() << ": loaded setting - k: '" << key << ", v: '" << value << "'." << endl;
                    }
                }
                file.close();
                cout << "Loaded configuration from '" << CONFIG_FILE << "'." << endl;
            }
            else
                cerr << getTitle() << ": warning - failed to load configuration at '" << CONFIG_FILE << "'..." << endl;
        }
        bool Configurator::save()
        {
            cout << "Attempting to save configuration..." << endl;
            ofstream file(CONFIG_FILE, ios::trunc);
            if(file.is_open())
            {
                // Write each setting to file
                string v;
                for(map<string, string>::iterator it = config.begin(); it != config.end(); it++)
                {
                    v = (*it).second;
                    std::replace(v.begin(), v.end(), '\n', CONFIG_NEWLINE_CHAR);
                    file << (*it).first << "=" << v << endl;
                }
                file.close();
                lastSave = Utils::getEpochTimeMs();
                cout << "Saved configuration to '" << CONFIG_FILE << "'." << endl;
                return true;
            }
            cerr << "Failed to save configuration to '" << CONFIG_FILE << "'!" << endl;
            wake(); // Ensure the main service thread is awake, since config save failed.
            return false;
        }
        void Configurator::_set(string key, string value)
        {
            config[key] = value;
            // Set the service to write the data later-on
            unsavedConfig = true;
            saveTime = Utils::getEpochTimeMs() + 1000LL;
            if(lastSave == 0LL)
                lastSave = Utils::getEpochTimeMs();
            wake(); // Wake the main thread ready for saving the file.
        }
        void Configurator::set(string key, string value)
        {
            // Replace new-line chars
            cout << getTitle() << ": setting '" << key << "', '" << value << "'..." << endl;
            // Save to config map collection
            unique_lock<mutex> lock(mutexThread);
            _set(key, value);
        }
        string Configurator::get(string key, string defaultValue)
        {
            unique_lock<mutex> lock(mutexThread);
            map<string, string>::iterator it = config.find(key);
            if(it == config.end())
            {
                _set(key, defaultValue);
                return defaultValue;
            }
            else
                return (*it).second;
        }
        int Configurator::get(string key, int defaultValue)
        {
            unique_lock<mutex> lock(mutexThread);
            map<string, string>::iterator it = config.find(key);
            // Check we found something...
            if(it == config.end())
            {
                _set(key, std::to_string(defaultValue));
                return defaultValue;
            }
            // Attempt to parse the value...
            int v;
            return Utils::tryParse((*it).second, v) ? v : defaultValue;
        }
        long long Configurator::get(string key, long long defaultValue)
        {
            unique_lock<mutex> lock(mutexThread);
            map<string, string>::iterator it = config.find(key);
            // Check we found something...
            if(it == config.end())
            {
                _set(key, std::to_string(defaultValue));
                return defaultValue;
            }
            // Attempt to parse the value...
            stringstream ss;
            long long result;
            ss << (*it).second;
            ss >> result;
            return result;
        }
        int Configurator::get(string key, int min, int max, int defaultValue)
        {
            int v = get(key, defaultValue);
            if(v < min || v > max)
                return defaultValue;
            else
                return v;
        }
    }
}