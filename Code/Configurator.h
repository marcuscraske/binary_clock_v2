
#ifndef CONFIGURATOR_H
#define	CONFIGURATOR_H

// Definitions ---------------------------------------------------------------->
#define CONFIG_SAVE_WAIT                1000LL                  // Time in milliseconds to wait to
                                                                // save data to disk.

#define CONFIG_SAVE_MAXWAIT             10000LL                 // The maximum time in milliseconds to
                                                                // wait to save data to disk; this avoids
                                                                // indefinite postponement.

#define CONFIG_FILE                     "binaryclock.config"    // The file location to store
                                                                // configuration on disk.

#define CONFIG_NEWLINE_CHAR             '$'                     // \n is replaced with this character.
#include <unistd.h>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "Definitions.h"
#include "IService.h"
#include "ServiceController.h"
#include "Utils.h"

using std::map;
using std::string;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::ios;
using std::pair;
using std::stringstream;

using BC::Services::IService;
using BC::ServiceController;
using BC::Utils;

namespace BC
{
    namespace Services
    {
        class Configurator : public IService
        {
        private:
            // Fields --------------------------------------------------------->
            bool unsavedConfig;                 // Indicates if the configuration has changed.
            long long saveTime,                 // The time at which to save the config.
                      lastSave;                 // The time data was last written.
            map<string, string> config;         // Stores all the settings key-values.
        public:
            // Constructors --------------------------------------------------->
            Configurator(ServiceController * const controller) : IService(controller), unsavedConfig(false), lastSave(0LL), saveTime(0LL) {}
        private:
            // Member Functions ----------------------------------------------->
            // -- Used to initially load the configuration from disk.
            void prerun();
            // -- Used for saving configuration after a time-period of
            // -- modification to avoid thrashing and repeated expensive I/O
            // -- operations.
            void run();
        public:
            // -- Saves the current configuration to disk. No mutex protection/
            // -- not thread-safe.
            bool save();
            // -- Loads configuration from disk; current configuration is completely
            // -- discarded. No mutex protection/not thread-safe.
            void load();
        public:
            // Member Functions - Mutators ------------------------------------>
            // -- Sets the value for a key; if the key already exists, it's
            // -- overwritten. Thread-safe.
            void set(string key, string value);
        private:
            // -- The actual underlying function used for setting kv's; not
            // -- thread-safe.
            void _set(string key, string value);
        public:
            // Member Functions - Accessors ----------------------------------->
            // -- Fetches a value; returns defaultValue if the key does not
            // -- exist. Thread-safe.
            string get(string key, string defaultValue);
            // -- Fetches a value as an integer; returns defaultValue if
            // -- the value could not be parsed or the key does not exist.
            // -- Thread-safe.
            int get(string key, int defaultValue);
            // -- Fetches a value as an integer; returns defaultValue if the
            // -- value could not be parsed, the key does not exist or the
            // -- numeric value is not inclusive of min and max. Thread-safe.
            int get(string key, int min, int max, int defaultValue);
            // -- Fetches a long-long value; returns defaultValue if the key is
            // -- not found.
            long long get(string key, long long defaultValue);
            // -- Fetches all of the available configuration; thread-safe.
            inline map<string, string> getAll()
            {
                return config;
            }
    public:
            string getTitle() { return SERVICETITLE_CONFIGURATOR; }
        };
    }
}

#endif	/* CONFIGURATOR_H */

