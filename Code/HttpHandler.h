#ifndef HTTPHANDLER_H
#define	HTTPHANDLER_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <string>

#include "Definitions.h"
#include "Structs.h"
#include "IClientHandler.h"
#include "HttpRequest.h"
#include "HttpResponse.h"

#include "Utils.h"
#include "Alarm.h"
#include "DiskCache.h"
#include "Templates.h"

#include "IModule.h"
#include "FunctionMapping.h"
#include "PageFunctions.h"
#include "Pages.h"


#include "Alarm.h"
#include "CountryLookup.h"
#include "Configurator.h"
#include "Display.h"
#include "Sensors.h"
#include "WebHttp.h"

using std::map;
using std::stringstream;
using std::string;
using std::ifstream;
using std::cout;
using std::cerr;
using std::endl;

using BC::Web::Core::Client;
using BC::Web::Http::HttpResponse;
using BC::Web::Http::HttpRequest;
using BC::Web::Http::FunctionMapping;
using BC::Web::Core::Data;

// Forward declarations
namespace BC
{
    namespace Services
    {
        class Alarm;
        class Configurator;
        class Display;
        class Sensors;
        class WebHttp;
        class CountryLookup;
    }
    namespace Web
    {
        namespace Http
        {
            class HttpRequest;
            class DiskCache;
        }
        namespace Modules
        {
            class IModule;
        }
    }
}
using BC::Services::Alarm;
using BC::Services::Configurator;
using BC::Services::Display;
using BC::Services::Sensors;
using BC::Services::WebHttp;
using BC::Services::CountryLookup;
using BC::Web::Core::IClientHandler;
using BC::Web::Http::HttpRequest;
using BC::Web::Http::DiskCache;
using namespace BC::Web::Modules;
           
#define HTTPHANDLER_DEFAULT_PAGE_DEFAULT        "home.bc"
#define HTTPHANDLER_DEFAULT_PAGE_NOTFOUND       "404.bct"

namespace BC
{
    namespace Web
    {
        namespace Http
        {
            class HttpHandler : public IClientHandler
            {
            private:
                // Fields ----------------------------------------------------->
                // -- Holds a mapping of function names to function pointers,
                // -- used for invoking functions from templates.
                map<string, FunctionMapping*> pageFunctions;
                // -- Holds the modules responsible for function mappings.
                vector<IModule*> pageModules;
                // -- Holds the epoch in m/s time at which the web-server started.
                long long epochStartTime;
                // -- The page used by default when no page is specified.
                string  pageDefault,
                // -- The page displayed for 404 / page not found responses.
                        pageNotFound;
                // -- External service pointers.
                Alarm           *serviceAlarm;
                Configurator    *serviceConfig;
                Display         *serviceDisplay;
                Sensors         *serviceSensors;
                WebHttp         *serviceWebHttp;
                CountryLookup   *serviceCountryLookup;
                // -- Used for caching items on the file-system.
                DiskCache       *diskCache;
            public:
                // Constructors ----------------------------------------------->
                HttpHandler() {}
                // Member Functions ------------------------------------------->
                void serviceStart(WebHttp *web);
                void serviceEnd(WebHttp *web);
                void handleClient(WebHttp *web, Client *client);
                // -- Returns a function pointer to the function associated with
                // -- the specified function name.
                FunctionMapping* functionGet(string function)
                {
                    map<string, FunctionMapping*>::iterator it = pageFunctions.find(function);
                    if(it != pageFunctions.end())
                        return (*it).second;
                    else
                        return 0;
                }
                inline void functionRegister(IModule *module, string functionName, FunctionMapping::func function)
                {
                    pageFunctions[functionName] = new FunctionMapping(module, function);
                }
                inline long long getEpochStartTimeMs() { return epochStartTime; }
                // Member Functions - Service Accessors ----------------------->
                Alarm *getService_Alarm()
                {
                    return serviceAlarm;
                }
                Configurator *getService_Configurator()
                {
                    return serviceConfig;
                }
                Display *getService_Display()
                {
                    return serviceDisplay;
                }
                Sensors *getService_Sensors()
                {
                    return serviceSensors;
                }
                WebHttp *getService_WebHttp()
                {
                    return serviceWebHttp;
                }
                DiskCache *getDiskCache()
                {
                    return diskCache;
                }
                CountryLookup *getCountryLookup()
                {
                    return serviceCountryLookup;
                }
            private:
                // -- Used to load and register a new module.
                void moduleLoad(IModule *module);
            };
        }
    }
}

#endif	/* HTTPHANDLER_H */

