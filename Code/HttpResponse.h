#ifndef HTTPRESPONSE_H
#define	HTTPRESPONSE_H

#include <map>
#include <string>
#include <sstream>
#include "Structs.h"

using BC::Web::Core::Data;
using std::map;
using std::string;
using std::stringstream;

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

