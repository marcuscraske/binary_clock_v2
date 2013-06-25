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
 *      File:           Templates.h
 *      Path:           BC/Web/Http/Templates.h
 * 
 *      Change-Log:
 *                      2013-06-25      Added header.
 * 
 * *****************************************************************************
 * Used by the web-server for formatting templates with the custom template
 * syntax markup for back-end function calls.
 * *****************************************************************************
 */
#ifndef TEMPLATES_H
#define	TEMPLATES_H

// Forward declarations
#include "HttpHandler.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
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
