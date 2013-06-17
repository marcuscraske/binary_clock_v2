
#ifndef TEMPLATES_H
#define	TEMPLATES_H

#include "HttpHandler.h"
#include "HttpRequest.h"
#include "HttpResponse.h"

// Forward declarations
namespace BC
{
    namespace Web
    {
        namespace Http
        {
            class HttpHandler;
            class HttpRequest;
            class HttpResponse;
        }
    }
}
using BC::Web::Http::HttpHandler;
using BC::Web::Http::HttpRequest;
using BC::Web::Http::HttpResponse;

namespace BC
{
    namespace Web
    {
        namespace Http
        {
            class Templates
            {
            public:
                static string format(HttpHandler * const handler, string html, Client *client, HttpRequest *request, HttpResponse *response);
            };
        }
    }
}

#endif	/* TEMPLATES_H */
