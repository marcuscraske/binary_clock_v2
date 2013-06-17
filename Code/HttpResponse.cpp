#include "HttpResponse.h"

namespace BC
{
    namespace Web
    {
        namespace Http
        {
            Data* HttpResponse::getPayload()
            {
                // Build header
                std::stringstream ssHeader;
                ssHeader << "HTTP/1.1 " << statusCode << " OK\r\nServer: " << server << "\r\nContent-Type: " << contentType << "; charset=utf-8\r\nContent-Length: " << dataLength << "\r\n";
                for(map<string, string>::iterator it = headers.begin(); it != headers.end(); it++)
                    ssHeader << (*it).first << ": " << (*it).second << "\r\n";
                ssHeader << "Connection: close\r\n\r\n";
                string header = ssHeader.str();
                int headerLength = header.length();
                char *dataHeader = new char[headerLength];
                for(int i = 0; i < headerLength; i++)
                    dataHeader[i] = header[i];
                // Compose final data to be sent (merge header and data char pointers)
                Data *finalData = new Data;
                finalData->length = dataLength + headerLength;
                finalData->data = new char[finalData->length];
                // Merge header and data
                for(int i = 0; i < headerLength; i++)
                    finalData->data[i] = dataHeader[i];
                for(int i = 0; i < dataLength; i++)
                    finalData->data[i + headerLength] = data[i];
                // Dispose local pointers
                delete[] dataHeader;
                
                return finalData;
            }
            HttpResponse::~HttpResponse()
            {
                if(data != 0)
                    delete[] data;
            }
            void HttpResponse::redirect(string url)
            {
                statusCode = 307;
                headers["Location"] = url;
            }
        }
    }
}