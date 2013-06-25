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
 *      File:           Structs.h
 *      Path:           BC/Web/Core/Structs.h
 * 
 *      Change-Log:
 *                      2013-06-25      Added header.
 * 
 * *****************************************************************************
 * A class of commonly used structs within this namespace.
 * *****************************************************************************
 */
#ifndef STRUCTS_H
#define	STRUCTS_H

#include <netinet/in.h>
#include <arpa/inet.h>

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

