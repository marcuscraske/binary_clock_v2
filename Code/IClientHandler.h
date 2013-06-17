#ifndef ICLIENTHANDLER_H
#define	ICLIENTHANDLER_H

#include <unistd.h>
#include <sys/socket.h>
#include <iostream>
#include <istream>
#include <string>
#include <sstream>

#include "Structs.h"
#include "WebHttp.h"

using std::istream;
using std::string;
using std::stringstream;

// Forward declarations
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

