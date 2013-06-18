#include "HttpHandler.h"
#include "Configurator.h"

using BC::Services::Configurator;

namespace BC
{
    namespace Web
    {
        namespace Http
        {
            void HttpHandler::serviceStart(WebHttp *web)
            {
                // Load configuration
                {
                    Configurator *c = static_cast<Configurator*>(web->getController()->getServiceByName(SERVICETITLE_CONFIGURATOR));
                    pageDefault = c->get(CONFIG__HTTPHANDLER_PAGE_DEFAULT, HTTPHANDLER_DEFAULT_PAGE_DEFAULT);
                    pageNotFound = c->get(CONFIG__HTTPHANDLER_PAGE_NOTFOUND, HTTPHANDLER_DEFAULT_PAGE_NOTFOUND);
                    // Setup service pointers
                    serviceAlarm = static_cast<Alarm*>(web->getController()->getServiceByName(SERVICETITLE_ALARM));
                    serviceConfig = static_cast<Configurator*>(web->getController()->getServiceByName(SERVICETITLE_CONFIGURATOR));
                    serviceDisplay = static_cast<Display*>(web->getController()->getServiceByName(SERVICETITLE_DISPLAY));
                    serviceSensors = static_cast<Sensors*>(web->getController()->getServiceByName(SERVICETITLE_SENSORS));
                    serviceWebHttp = static_cast<WebHttp*>(web->getController()->getServiceByName(SERVICETITLE_WEBHTTP));
                    serviceCountryLookup = static_cast<CountryLookup*>(web->getController()->getServiceByName(SERVICETITLE_COUNTRYLOOKUP));
                    serviceRelayBoard = static_cast<RelayBoard*>(web->getController()->getServiceByName(SERVICETITLE_RELAYBOARD));
                }
                // Load page function modules
                moduleLoad(new PageFunctions());
                moduleLoad(new Pages());
                
                // Load disk cache
                diskCache = new DiskCache(web->getController(), Utils::getBasePath() + "/Website");
                
                // Set startup time
                epochStartTime = Utils::getEpochTimeMs();
                std::cout << "HTTP handler enabled." << std::endl;
            }
            void HttpHandler::serviceEnd(WebHttp *web)
            {
                // Dispose function mappings and modules
                for(map<string, FunctionMapping*>::iterator it=pageFunctions.begin(); it != pageFunctions.end(); it++)
                    delete (*it).second;
                pageFunctions.clear();
                
                for(vector<IModule*>::iterator it=pageModules.begin(); it != pageModules.end(); it++)
                    delete (*it);
                pageModules.clear();
                
                // Null-route pointers for safety
                serviceAlarm = 0;
                serviceConfig = 0;
                serviceDisplay = 0;
                serviceSensors = 0;
                serviceWebHttp = 0;
                
                std::cout << "HTTP handler disabled." << std::endl;
            }
            void HttpHandler::moduleLoad(IModule* module)
            {
                if(module->moduleRegister(this))
                    pageModules.push_back(module);
                else
                    delete module;
            }
            void HttpHandler::handleClient(WebHttp *web, Client *client)
            {
                // Read HTTP headers
                HttpRequest *request = new HttpRequest(this, web, client);
                HttpResponse *response = new HttpResponse();
                cout << "HTTP handler - new request - " << client->ipAddress << ":" << client->addrClient.sin_port << " - /" << request->getPath() << endl;
                
                // Check the extension
                int ind = request->getPath().find_first_of('?');
                string path = request->getPath().substr(0, ind);
                ind = path.find_last_of('.');
                string ext = ind == string::npos || ind >= path.length() - 1 ? "" : path.substr(ind+1);
                if(ext.length() == 0 || ext == "bc")
                {
                    // Fetch the associated page and format it
                    DiskCacheItem *item = diskCache->fetch(path.length() == 0 ? pageDefault : ext.length() == 0 ? path + ".bc" : path);
                    if(item != 0)
                    {
                        string page = string(item->getData(), item->getLength());
                        // Format the custom markup language within the file
                        page = Templates::format(this, page, client, request, response);
                        // Copy the string to a new char-array as the response
                        response->data = new char[page.length()];
                        for(int i = 0; i < page.length(); i++)
                            response->data[i] = page[i];
                        response->dataLength = page.length();
                    }
                }
                else if(ext == "css" || ext == "js" || ext == "gif" || ext == "jpg" || ext == "png" || ext == "bmp")
                {
                    // Load the image from cache
                    DiskCacheItem *item = diskCache->fetch(path);
                    if(item != 0)
                    {
                        response->dataLength = item->getLength();
                        response->data = new char[response->dataLength];
                        Utils::arrayClone(item->getData(), response->data, response->dataLength);
                        response->contentType = ext == "css" ? "text/css" : ext == "js" ? "text/javascript" : "image/" + ext;
                    }
                }
                // Check a response has been set, else a 404 has occurred
                if(response->data == 0)
                {
                    // Display the 404 page
                    string pageData;
                    // -- Attempt to load it; this is added protection to avoid e.g. crashing the web-service
                    if(diskCache->fetchLoad(pageNotFound, pageData))
                        pageData = Templates::format(this, pageData, client, request, response);
                    else
                        pageData = "Failed to load 404 page...this sounds bad ;___;...";
                    response->data = new char[pageData.length()];
                    for(int i = 0; i < pageData.length(); i++)
                           response->data[i] = pageData[i];
                    response->dataLength = pageData.length();
                    response->statusCode = 404;
                }
                // Write the response to socket
                Data *payload = response->getPayload();
                if(!writeSocketData(client, payload->data, payload->length))
                    cerr << "HTTP handler - Failed to write data to client (path: " << request->getPath() << ", client: " << client->ipAddress << ":" << client->addrClient.sin_port << ")!" << endl;
                // Dispose resources
                delete request;
                delete response;
                delete payload;
            }
        }
    }
}