
#include "ClientSocket.h"
#include "Utils.h"

namespace BC
{
    namespace Web
    {
        namespace Core
        {
            bool ClientSocket::_connect(in_addr ip, int port)
            {
                // Create client socket
                if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
                {
                    cerr << "Failed to create client socket - error-code: '" << sock << "'!" << endl;
                    close(sock);
                    return false;
                }
                // Setup connection details
                struct sockaddr_in addr;
                addr.sin_family = AF_INET;
                addr.sin_port = htons(port);
                addr.sin_addr = ip;
                // Connect to destination
                int connCode = connect(sock, (sockaddr*)&addr, sizeof(addr));
                // Check if we were able to connect by checking the connection code
                if(connCode != 0)
                {
                    cerr << "Failed to connect to '" << inet_ntoa(ip) << ":" << port << "' - error-code '" << connCode << "'!" << endl;
                    close(sock);
                    return false;
                }
                sockCreated = true;
                return true;
            }
            bool ClientSocket::connectToHost(string host, int port)
            {
                // Lookup the host info
                struct hostent *hinfo;
                if((hinfo = gethostbyname(host.c_str())) == 0)
                {
                    std::cerr << "Failed to resolve host information for host-name '" << host << "'!" << endl;
                    return false;
                }
                // Connect to the first IP found
                struct in_addr **ips = (struct in_addr**) hinfo->h_addr_list;
                if(ips[0] == 0)
                    return false;
                else
                    return _connect(*ips[0], port);
            }
            bool ClientSocket::connectToIP(string ip, int port)
            {
                // Create client socket
                if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
                {
                    cerr << "Failed to create client socket - error-code: '" << sock << "'!" << endl;
                    close(sock);
                    return false;
                }
                // Setup connection details
                struct sockaddr_in addr;
                addr.sin_family = AF_INET;
                addr.sin_port = htons(port);
                inet_aton(ip.c_str(), &addr.sin_addr);
                // Connect to destination
                int connCode = connect(sock, (sockaddr*)&addr, sizeof(addr));
                // Check if we were able to connect by checking the connection code
                if(connCode != 0)
                {
                    cerr << "Failed to connect to '" << ip << ":" << port << "' - error-code '" << connCode << "'!" << endl;
                    close(sock);
                    return false;
                }
                sockCreated = true;
                return true;
            }
            void ClientSocket::disconnect()
            {
                close(sock);
            }
            bool ClientSocket::write(const char* data, int length)
            {
                return send(sock, data, length, 0) == length;
            }
            bool ClientSocket::write(stringstream &ss, bool clear)
            {
                string data = ss.str();
                if(clear)
                {
                    ss.str(std::string());
                    ss.clear();
                }
                return write(data.c_str(), data.length());
            }
            bool ClientSocket::read(char *buffer, int length)
            {
                return recv(sock, buffer, length, 0) > 0;
            }
            int ClientSocket::readWithLength(char *buffer, int length)
            {
                return recv(sock, buffer, length, 0);
            }
            int ClientSocket::readIntoStream(ostream &os)
            {
                char *buffer = new char[1024];
                // Set the socket to not block
                fd_set i;
                FD_ZERO(&i);
                FD_SET(sock, &i);
                struct timeval tout;
                tout.tv_sec = 1; // 1 second time-out between polls
                tout.tv_usec = 0; // 1 m/s = 1000 u/s
                int c = select(sock+1, &i, 0, 0, &tout);
                if(c == -1)
                {
                    delete buffer;
                    return 0;
                }
                // Read until less than 1
                int totalBytes = 0;
                int bytes;
                while((bytes = recv(sock, buffer, 1024, 0)) > 0 || totalBytes == 0)
                {
                    if(bytes > 0)
                    {
                        os.write(buffer, bytes);
                        totalBytes += bytes;
                    }
                }
                return totalBytes;
            }
        }
    }
}