#include "IClientHandler.h"

namespace BC
{
    namespace Web
    {
        namespace Core
        {
            Data* IClientHandler::readSocketData(WebHttp *web, Client *client, int bufferLimit)
            {
                Data *data = new Data;
                data->data = new char[bufferLimit];
                data->length = read(client->socket, data->data, bufferLimit);
                if(data->length < 0)
                {
                    std::cerr << "Client handler - failed to read data of client socket (" << client->addrClient.sin_addr.s_addr << ":" << client->addrClient.sin_port << ")!" << std::endl;
                    delete data;
                    return 0;
                }
                else
                    return data;
            }
            bool IClientHandler::writeSocketData(Client *client, char *data, int length)
            {
                // Write the data to the socket
                if(write(client->socket, data, length) < 0)
                {
                    std::cerr << "Client handler - failed to write data to client socket (" << client->addrClient.sin_addr.s_addr << ":" << client->addrClient.sin_port << ")!" << std::endl;
                    return false;
                }
                return true;
            }
        }
    }
}
