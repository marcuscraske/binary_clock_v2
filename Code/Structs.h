#ifndef STRUCTS_H
#define	STRUCTS_H

#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>

namespace BC
{
    namespace Web
    {
        namespace Core
        {
            struct Client
            {
                int threadId;
                int socket;
                sockaddr_in addrClient;
                char ipAddress[INET_ADDRSTRLEN];
            };
            struct Data
            {
                int length;
                char *data;
                ~Data()
                {
                    if(data != 0)
                        delete[] data;
                }
            };
        }
    }
}

#endif	/* STRUCTS_H */

