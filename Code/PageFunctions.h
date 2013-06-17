
#ifndef PAGEFUNCTIONS_H
#define	PAGEFUNCTIONS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <functional>

#include "HttpResponse.h"
#include "HttpRequest.h"

using std::ifstream;
using std::stringstream;
using std::map;
using std::string;

using BC::Services::WebHttp;
using BC::Web::Http::HttpHandler;
using BC::Web::Http::HttpRequest;
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

