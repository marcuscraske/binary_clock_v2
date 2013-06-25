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
 *      File:           ClientSocket.h
 *      Path:           BC/Web/Core/ClientSocket.h
 * 
 *      Change-Log:
 *                      2013-06-25      Added header.
 * 
 * *****************************************************************************
 * A class used for connecting to a specified destination using TCP/IP
 * for sending and receiving data.
 * *****************************************************************************
 */
#ifndef CLIENTSOCKET_H
#define	CLIENTSOCKET_H

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <iostream>
using std::cerr;
using std::endl;

#include <ostream>
using std::ostream;

#include <string>
using std::string;

#include <sstream>
using std::stringstream;

#include <netdb.h>
#include <fcntl.h>

namespace BC
{
    namespace Web
    {
        namespace Core
        {
            class ClientSocket
            {
            public:
                struct Data
                {
                    char *data;
                    int length;
                };
            private:
                // Fields ----------------------------------------------------->
                int sock;
                bool sockCreated;
            public:
                // Constructors ----------------------------------------------->
                ClientSocket() : sockCreated(false) { }
                // Destructors ------------------------------------------------>
                ~ClientSocket()
                {
                    if(sockCreated)
                        close(sock);
                }
            private:
                // Member Functions ------------------------------------------->
                bool _connect(in_addr ip, int port);
            public:
                bool connectToHost(string host, int port);
                bool connectToIP(string ip, int port);
                void disconnect();
                bool write(const char* data, int length);
                bool write(stringstream &ss, bool clear);
                bool read(char *buffer, int length);
                int readWithLength(char *buffer, int length);
                int readIntoStream(ostream &os);
            };
        }
    }
}

#endif	/* CLIENTSOCKET_H */

