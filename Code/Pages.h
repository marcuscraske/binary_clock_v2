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
 *      File:           Pages.h
 *      Path:           BC/Web/Modules/Pages.h
 * 
 *      Change-Log:
 *                      2013-06-25      Added header.
 * 
 * *****************************************************************************
 * A module for the web-server used for rendering pages.
 * *****************************************************************************
 */
#ifndef PAGES_H
#define	PAGES_H

#include "Definitions.h"

#include <iostream>
#include <fstream>
using std::ifstream;

#include <map>
using std::map;

#include <string>
using std::string;

#include <pqxx/pqxx>
using namespace pqxx;

#include <sstream>
using std::stringstream;

#include <stdlib.h>

#include "WebService.h"
using BC::Services::WebService;

#include "Structs.h"
using BC::Web::Core::Client;
using BC::Web::Core::Data;

#include "HttpRequest.h"
using BC::Web::Http::HttpRequest;

#include "HttpResponse.h"
using BC::Web::Http::HttpResponse;

#include "IModule.h"
using BC::Web::Modules::IModule;

#include "SystemInfo.h"
using BC::Web::Modules::SystemInfo;

#include "HttpHandler.h"
using BC::Web::Http::HttpHandler;

#include "Utils.h"
using BC::Utils;

#include "Alarm.h"
using BC::Services::Alarm;

#include "RelayBoard.h"
using BC::Services::RelayBoard;

// Forward declarations
#include "Templates.h"
namespace BC
{
    namespace Web
    {
        namespace Http
        {
            class Templates;
        }
    }
}
using BC::Web::Http::Templates;

namespace BC
{
    namespace Web
    {
        namespace Modules
        {
            class Pages : public IModule
            {
            private:
                string dbConnectionString;      // The connection string used to connect to the database.
                int guestbookThreshold;         // The threshold of posts per an hour.
            public:
                bool moduleRegister(HttpHandler * const handler);
                static string pageAdmin(HttpHandler * const handler, IModule *module, Client *client, HttpRequest *request, HttpResponse *response, string arguments, string &pageUpper, string &pageLower);
                static void pageAdmin_alarm(string &content, HttpHandler * const handler, IModule *module, Client *client, HttpRequest *request, HttpResponse *response, string arguments, string &pageUpper, string &pageLower);
                static void pageAdmin_settings(string &content, HttpHandler * const handler, IModule *module, Client *client, HttpRequest *request, HttpResponse *response, string arguments, string &pageUpper, string &pageLower);
                static void pageAdmin_services(string &content, HttpHandler * const handler, IModule *module, Client *client, HttpRequest *request, HttpResponse *response, string arguments, string &pageUpper, string &pageLower);
                static void pageAdmin_diskCache(string &content, HttpHandler * const handler, IModule *module, Client *client, HttpRequest *request, HttpResponse *response, string arguments, string &pageUpper, string &pageLower);
                static void pageAdmin_relayBoard(string &content, HttpHandler * const handler, IModule *module, Client *client, HttpRequest *request, HttpResponse *response, string arguments, string &pageUpper, string &pageLower);
            private:
                static string pageAdmin_relayBoard_buildConditions(int index, string &templateCondition, vector<RelayCondition> conditions);
            public:
                static string pageGuestbook(HttpHandler * const handler, IModule *module, Client *client, HttpRequest *request, HttpResponse *response, string arguments, string &pageUpper, string &pageLower);
            private:
                static string pageGuestbook_recursiveRenderSubPosts(result::const_iterator postTuple, string &templateContainer, string &templatePost, work *w);
                static string pageGuestbook_renderPost(result::const_iterator tuple, string &templatePost);
            public:
                static string pageTools_SystemInfo(HttpHandler * const handler, IModule *module, Client *client, HttpRequest *request, HttpResponse *response, string arguments, string &pageUpper, string &pageLower);
                static string pageTools_API(HttpHandler * const handler, IModule *module, Client *client, HttpRequest *request, HttpResponse *response, string arguments, string &pageUpper, string &pageLower);
                static string pageTools_RandomPornVideo(HttpHandler * const handler, IModule *module, Client *client, HttpRequest *request, HttpResponse *response, string arguments, string &pageUpper, string &pageLower);
                static string pageTools_RandomGenerator(HttpHandler * const handler, IModule *module, Client *client, HttpRequest *request, HttpResponse *response, string arguments, string &pageUpper, string &pageLower);
            private:
                static bool isLocalNetwork(char *ip);
            };
        }
    }
}

#endif	/* PAGES_H */

