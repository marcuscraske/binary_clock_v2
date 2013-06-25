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
 *      File:           HttpResponse.h
 *      Path:           BC/Web/Http/HttpResponse.h
 * 
 *      Change-Log:
 *                      2013-06-25      Added header.
 * 
 * *****************************************************************************
 * Used by the web-server to hold the response data for a client's request.
 * *****************************************************************************
 */
#ifndef HTTPRESPONSE_H
#define	HTTPRESPONSE_H

#include <map>
using std::map;

#include <string>
using std::string;

#include <sstream>
using std::stringstream;

#include "Structs.h"
using BC::Web::Core::Data;
using BC::Web::Core::Client;

namespace BC
{
    namespace Web
    {
        namespace Http
        {
            class HttpResponse
            {
                // Fields
            public:
                map<string, string> headers;
                char* data;
                int dataLength;
                int statusCode;
                string contentType;
                string server;
                // Member functions - constructors
                HttpResponse() : data(0), dataLength(0), statusCode(200), contentType("text/html"), server("BinaryClockV2") {}
                ~HttpResponse();
                // Member functions
                Data* getPayload();
                void redirect(string url);
            };
        }
    }
}

#endif	/* HTTPRESPONSE_H */

