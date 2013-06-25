/*                       ____               ____________
 *                      |    |             |            |
 *                      |    |             |    ________|
 *                      |    |             |   |
 *                      |    |             |   |    
 *                      |    |             |   |    ____
 *                      |    |             |   |   |    |
 *                      |    |_______      |   |___|    |
 *                      |            |  _  |            |
 *                      |____________| |_| |____________|
 *                        
 *      Author(s):      limpygnome (Marcus Craske)              limpygnome@gmail.com
 * 
 *      License:        Creative Commons Attribution-ShareAlike 3.0 Unported
 *                      http://creativecommons.org/licenses/by-sa/3.0/
 * 
 *      File:           HttpHandler.h
 *      Path:           BC/Web/Http/HttpHandler.h
 * 
 *      Change-Log:
 *                      2013-06-25      Added header.
 * 
 * *****************************************************************************
 * The handler responsible for implementing the web-server using HTTP/1.1; this
 * also acts as the bridge for functions to access other services.
 * *****************************************************************************
 */
#ifndef HTTPHANDLER_H
#define	HTTPHANDLER_H

#include "Definitions.h"

#include <iostream>
using std::ifstream;
using std::cout;
using std::cerr;
using std::endl;

#include <sstream>
using std::stringstream;

#include <fstream>

#include <map>
using std::map;

#include <string>
using std::string;

#include "Structs.h"
using BC::Web::Core::Client;
using BC::Web::Core::Data;

#include "IClientHandler.h"
using BC::Web::Core::IClientHandler;

#include "Utils.h"
using BC::Utils;

#include "Templates.h"

#include "FunctionMapping.h"
using BC::Web::Http::FunctionMapping;

#include "HttpResponse.h"
using BC::Web::Http::HttpResponse;

#include "PageFunctions.h"
#include "Pages.h"
using namespace BC::Web::Modules;

// Forward declarations
#include "Alarm.h"
#include "CountryLookup.h"
#include "Configurator.h"
#include "Display.h"
#include "Sensors.h"
#include "WebHttp.h"
#include "RelayBoard.h"

#include "HttpRequest.h"
#include "DiskCache.h"

#include "IModule.h"

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
        class RelayBoard;
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
using BC::Services::RelayBoard;
using BC::Web::Core::IClientHandler;
using BC::Web::Http::HttpRequest;
using BC::Web::Http::DiskCache;
           
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
                RelayBoard      *serviceRelayBoard;
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
                inline Alarm *getService_Alarm()
                {
                    return serviceAlarm;
                }
                inline Configurator *getService_Configurator()
                {
                    return serviceConfig;
                }
                inline Display *getService_Display()
                {
                    return serviceDisplay;
                }
                inline Sensors *getService_Sensors()
                {
                    return serviceSensors;
                }
                inline WebHttp *getService_WebHttp()
                {
                    return serviceWebHttp;
                }
                inline DiskCache *getDiskCache()
                {
                    return diskCache;
                }
                inline CountryLookup *getCountryLookup()
                {
                    return serviceCountryLookup;
                }
                inline RelayBoard *getRelayBoard()
                {
                    return serviceRelayBoard;
                }
            private:
                // -- Used to load and register a new module.
                void moduleLoad(IModule *module);
            };
        }
    }
}

#endif	/* HTTPHANDLER_H */

