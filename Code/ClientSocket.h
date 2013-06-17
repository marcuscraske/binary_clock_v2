
#ifndef CLIENTSOCKET_H
#define	CLIENTSOCKET_H

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <ostream>
#include <string>
#include <sstream>
#include <netdb.h>
#include <fcntl.h>

using std::cerr;
using std::endl;
using std::string;
using std::stringstream;
using std::ostream;

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

