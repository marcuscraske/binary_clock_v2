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
 *      File:           IClientHandler.h
 *      Path:           BC/Web/Core/IClientHandler.h
 * 
 *      Change-Log:
 *                      2013-06-25      Added header.
 * 
 * *****************************************************************************
 * The base class for a handler. The web-server uses a handler for handling
 * clients; thus the web-server could technically be turned into any other
 * service besides HTTP. This architecture has been implemented in-case HTTP
 * is removed or an additional service is implemented in the future.
 * *****************************************************************************
 */
#ifndef ICLIENTHANDLER_H
#define	ICLIENTHANDLER_H

#include <unistd.h>
#include <sys/socket.h>

#include <iostream>
#include <istream>
using std::istream;

#include <string>
using std::string;

#include <sstream>
using std::stringstream;

#include "Structs.h"
using BC::Web::Core::Client;
using BC::Web::Core::Data;

// Forward declarations
#include "WebHttp.h"
namespace BC
{
    namespace Services
    {
        class WebHttp;
    }
}
using BC::Services::WebHttp;

namespace BC
{
    namespace Web
    {
        namespace Core
        {
            class IClientHandler
            {
            public:
                IClientHandler() {}
                /**
                 * Writes data to the client socket.
                 */
                bool writeSocketData(Client *client, char *data, int length);
                /**
                 * Attempts to read data from the socket.
                 * @param listener The listener.
                 * @param client The client of which to read data from.
                 * @return Null if an error occurred.
                 */
                Data* readSocketData(WebHttp *web, Client *client, int bufferLimit);
                virtual void serviceStart(WebHttp *web) {}
                virtual void serviceEnd(WebHttp *web) {}
                virtual void handleClient(WebHttp *web, Client *client) { }
            };
        }
    }
}
#endif	/* ICLIENTHANDLER_H */

