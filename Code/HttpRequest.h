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
 *      File:           HttpRequest.h
 *      Path:           BC/Web/Http/HttpRequest.h
 * 
 *      Change-Log:
 *                      2013-06-25      Added header.
 * 
 * *****************************************************************************
 * Used to represent a client's HTTP request data to the web server.
 * *****************************************************************************
 */
#ifndef HTTPREQUEST_H
#define	HTTPREQUEST_H

#include <map>
using std::map;

#include <string>
using std::string;

#include "Structs.h"
using BC::Web::Core::Client;
using BC::Web::Core::Data;

#include "HttpHandler.h"
using BC::Web::Http::HttpHandler;

#include "WebHttp.h"
using BC::Services::WebHttp;

#include "Utils.h"
using BC::Utils;

// Forward declarations
namespace BC
{
    namespace Web
    {
        namespace Http
        {
            class HttpHandler;
        }
    }
}
using BC::Web::Http::HttpHandler;

namespace BC
{
    namespace Web
    {
        namespace Http
        {
            class HttpRequest
            {
            public:
                // Enums
                enum HttpMethod
                {
                    Unknown = 0,
                    Get = 1,
                    Post = 2
                };
            private:
                // Fields
                HttpMethod method;
                string path;
                long long timeStart;
            public:
                map<string, string> headers;
                map<string, string> queryStrings;
                map<string, string> formData;
            public:
                // Member functions - constructors
                HttpRequest(HttpHandler* handler, WebHttp* web, Client* client);
            public:
                void parseQueryString()
                {
                    // Check a query-string has been specified
                    int ind = path.find_first_of('?');
                    if(ind == string::npos || ind >= path.length() - 1)
                        return;
                    
                    // Get the querystring segment and begin parsing key-values
                    parseData(path.substr(ind+1), queryStrings);
                }
                // Accessors
                inline HttpMethod getMethod() const
                {
                    return method;
                }
                inline string getPath() const
                {
                    return path;
                }
                inline long long getTimeStart() const
                {
                    return timeStart;
                }
            private:
                void parseData(string data, map<string, string> &dest);
            };
        }
    }
}

#endif	/* HTTPREQUEST_H */

