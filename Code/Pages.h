
#ifndef PAGES_H
#define	PAGES_H

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <pqxx/pqxx>
#include <sstream>

#include "Definitions.h"
#include "WebHttp.h"
#include "Structs.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "IModule.h"
#include "Templates.h"
#include "SystemInfo.h"

using std::map;
using std::string;
using std::stringstream;
using std::ifstream;
using namespace pqxx;

using BC::Services::WebHttp;
using BC::Web::Core::Client;
using BC::Web::Modules::IModule;
using BC::Web::Http::HttpHandler;
using BC::Web::Http::HttpRequest;
using BC::Web::Http::HttpResponse;
using BC::Web::Modules::SystemInfo;

// Forward declarations
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

#define MODULES_PAGES_DEFAULT_ADMIN_IP_START            "10.0.0."
#define MODULES_PAGES_DEFAULT_HOST                      "127.0.0.1"
#define MODULES_PAGES_DEFAULT_PORT                      5432
#define MODULES_PAGES_DEFAULT_DATABASE                  "main"
#define MODULES_PAGES_DEFAULT_USER                      "postgres"
#define MODULES_PAGES_DEFAULT_PASS                      ""

#define MODULES_PAGES_GUESTBOOK_POSTSPERPAGE            4
#define MODULES_PAGES_GUESTBOOK_NAME_MIN                0
#define MODULES_PAGES_GUESTBOOK_NAME_MAX                24
#define MODULES_PAGES_GUESTBOOK_MESSAGE_MIN             3
#define MODULES_PAGES_GUESTBOOK_MESSAGE_MAX             512
#define MODULES_PAGES_GUESTBOOK_DEFAULT_POST_THRESHOLD  3

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

