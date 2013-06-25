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
 *      File:           PageFunctions.h
 *      Path:           BC/Web/Modules/PageFunctions.h
 * 
 *      Change-Log:
 *                      2013-06-25      Added header.
 * 
 * *****************************************************************************
 * A module with many common page functions for the web-server.
 * *****************************************************************************
 */
#ifndef PAGEFUNCTIONS_H
#define	PAGEFUNCTIONS_H

#include <iostream>
#include <fstream>
using std::ifstream;

#include <sstream>
using std::stringstream;

#include <string>
using std::string;

#include <functional>

#include <map>
using std::map;

#include "WebHttp.h"
using BC::Services::WebHttp;

#include "HttpHandler.h"
using BC::Web::Http::HttpHandler;

#include "HttpRequest.h"
using BC::Web::Http::HttpRequest;

#include "HttpResponse.h"
using BC::Web::Http::HttpResponse;

namespace BC
{
    namespace Web
    {
        namespace Modules
        {
            class PageFunctions : public IModule
            {
            public:
                bool moduleRegister(HttpHandler * const handler);
                static string include(HttpHandler * const handler, IModule *module, Client *client, HttpRequest *request, HttpResponse *response, string arguments, string &pageUpper, string &pageLower);
                static string getTemperature(HttpHandler * const handler, IModule *module, Client *client, HttpRequest *request, HttpResponse *response, string arguments, string &pageUpper, string &pageLower);
                static string getLight(HttpHandler * const handler, IModule *module, Client *client, HttpRequest *request, HttpResponse *response, string arguments, string &pageUpper, string &pageLower);
                static string getIpAddress(HttpHandler * const handler, IModule *module, Client *client, HttpRequest *request, HttpResponse *response, string arguments, string &pageUpper, string &pageLower);
                static string getAlarms(HttpHandler * const handler, IModule *module, Client *client, HttpRequest *request, HttpResponse *response, string arguments, string &pageUpper, string &pageLower);
                static string getFieldTime(HttpHandler * const handler, IModule *module, Client *client, HttpRequest *request, HttpResponse *response, string arguments, string &pageUpper, string &pageLower);
                static string getServices(HttpHandler * const handler, IModule *module, Client *client, HttpRequest *request, HttpResponse *response, string arguments, string &pageUpper, string &pageLower);
                static string getSetting(HttpHandler * const handler, IModule *module, Client *client, HttpRequest *request, HttpResponse *response, string arguments, string &pageUpper, string &pageLower);
                static string getUptime(HttpHandler * const handler, IModule *module, Client *client, HttpRequest *request, HttpResponse *response, string arguments, string &pageUpper, string &pageLower);
                // -- Returns the amount of time taken to process the request thus-far.
                static string getProcessTime(HttpHandler * const handler, IModule *module, Client *client, HttpRequest *request, HttpResponse *response, string arguments, string &pageUpper, string &pageLower);
            };
        }
    }
}

#endif	/* PAGEFUNCTIONS_H */

