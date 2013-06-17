#include "HttpRequest.h"

#include <iostream>
#include <sstream>
#include "Structs.h"

using std::stringstream;
using BC::Web::Core::Data;

#define HTTP_HEADER_LIMIT_BYTES 8190

namespace BC
{
    namespace Web
    {
        namespace Http
        {
            HttpRequest::HttpRequest(HttpHandler* handler, WebHttp* web, Client* client)
            {
                timeStart = Utils::getEpochTimeMs();
                // Read data from stream, place into stringstream for processing
                Data *data = handler->readSocketData(web, client, HTTP_HEADER_LIMIT_BYTES);
                // Set null terminator
                data->data[data->length >= HTTP_HEADER_LIMIT_BYTES ? HTTP_HEADER_LIMIT_BYTES - 1 : data->length] = '\0';
                // Read data as string
                stringstream ssData(stringstream::in | stringstream::out);
                ssData << data->data;
                delete data;
                // Read path line
                string line;
                if(!getline(ssData, line, '\n'))
                    path = "";
                else
                {
                    stringstream ssLine(stringstream::in | stringstream::out);
                    ssLine << line;
                    // Method
                    string sTemp;
                    if(ssLine >> sTemp)
                    {
                        if(sTemp == "GET")
                            method = HttpMethod::Get;
                        else if(sTemp == "POST")
                            method = HttpMethod::Post;
                        else
                            method = HttpMethod::Unknown;
                        // Get path and check HTTP/1.1
                        if(!(ssLine >> path) || path.substr(0, 1) != "/" || !(ssLine >> sTemp) || sTemp != "HTTP/1.1")
                            path = "invalid_client";
                        else
                            // Strip tailing slash
                            path = path.length() > 1 ? path.substr(1) : "";
                    }
                    else
                        path = "invalid_client";
                }
                // Read header lines
                string key, value;
                int kvSeparatorColon;
                bool endOfHeaders = false;
                stringstream ssFormData;
                while(getline(ssData, line, '\n'))
                {
                    if(endOfHeaders)
                        ssFormData << line;
                    else if(line.length() > 1)
                    {
                        kvSeparatorColon = line.find_first_of(':');
                        if(kvSeparatorColon != string::npos && kvSeparatorColon + 4 < line.length())
                        {
                            key = line.substr(0, kvSeparatorColon);
                            value = line.substr(kvSeparatorColon + 2, line.length() - (3 + kvSeparatorColon));
                        }
                    }
                    else
                        endOfHeaders = true;
                }
                // Parse form data
                parseData(ssFormData.str(), formData);
            }
            void HttpRequest::parseData(string querySeg, map<string, string> &dest)
            {
                // Check we have't already parsed it
                if(dest.size() != 0)
                    return;
                // Loop whilst we still have keys (key=value&key=value&...)
                int ind, ind2;
                string t, key, value;
                do
                {
                    ind = querySeg.find_first_of('&');
                    if(ind != string::npos)
                        t = querySeg.substr(0, ind);
                    else
                        t = querySeg;
                    ind2 = querySeg.find_first_of('=');
                    if(ind2 != string::npos && ind2 < t.length())
                    {
                        key = t.substr(0, ind2);
                        value = ind2+1 < t.length() ? t.substr(ind2+1) : "";
                        dest[key] = value;
                    }
                    if(ind+1 < querySeg.length()-1)
                        querySeg = querySeg.substr(ind+1);
                    else
                        querySeg = "";
                }
                while(ind != string::npos);
            }
        }
    }
}