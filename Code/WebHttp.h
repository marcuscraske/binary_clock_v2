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
 *      File:           WebHttp.h
 *      Path:           BC/Services/WebHttp.h
 * 
 *      Change-Log:
 *                      2013-06-25      Added header.
 * 
 * *****************************************************************************
 * The web service, responsible for running the binary clock's internal
 * web-server.
 * *****************************************************************************
 */
#ifndef WEBHTTP_H
#define	WEBHTTP_H

#define WEBHTTP_DEFAULT_PORT                    5000
#define WEBHTTP_DEFAULT_SOCKETS_BACKLOG         64
#define WEBHTTP_DEFAULT_THREADS_MAX             64
#define WEBHTTP_SOCKET_POLLRATE_MS              500

#include "Definitions.h"

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include <string>
using std::string;

#include <sstream>
using std::stringstream;

#include <sys/sysinfo.h>

#include <thread>
#include <pthread.h>
using std::thread;

#include <mutex>
using std::mutex;
using std::lock_guard;

#include <condition_variable>
using std::condition_variable;

#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include "IService.h"
using BC::Services::IService;

#include "IClientHandler.h"
using BC::Web::Core::IClientHandler;

#include "Structs.h"
using BC::Web::Core::Client;
using BC::Web::Core::Data;

namespace BC
{
    namespace Web
    {
        namespace Core
        {
            class IClientHandler;
        }
    }
}
using BC::Web::Core::IClientHandler;

namespace BC
{
    namespace Services
    {   
        class WebHttp : public IService
        {
        private:
            // Fields --------------------------------------------------------->
                                // -- File-descriptor of the socket used for
                                // -- listening for requests.
            int                 socketListen,
                                // -- The port of the web-server.
                                port,
                                // -- The maximum number of client sockets held
                                // -- in the buffer before being handled.
                                socketsBacklog,
                                // -- The maximum number of threads to be
                                // -- created to handle client requests.
                                threadsMax,
                                // -- The current number of available threads.
                                threadsAvailable;
                                // -- The mutex used for locking the threads
                                // -- collection.
            mutex               threadsMutex;
                                // -- Used for blocking the service thread until
                                // -- a listening thread is released.
            condition_variable  threadsMutexCondition;
                                // -- The collection of listening threads.
            thread              **threads;
                                // -- Indicates if a thread is dead/available,
                                // -- with each index corresponding to the same
                                // -- index of a thread in the threads
                                // -- collection.
            bool                *threadsDead;
                                // -- A pointer to the client handler
                                // -- responsible for handling clients.
            IClientHandler      *clientHandler;
        public:
            // Constructors --------------------------------------------------->
            WebHttp(ServiceController * const controller, IClientHandler *clientHandler):port(WEBHTTP_DEFAULT_PORT), socketsBacklog(WEBHTTP_DEFAULT_SOCKETS_BACKLOG),
                        threadsMax(WEBHTTP_DEFAULT_THREADS_MAX), threadsAvailable(WEBHTTP_DEFAULT_THREADS_MAX), clientHandler(clientHandler),
                    IService(controller)
            { }
            // Destructors ---------------------------------------------------->
            ~WebHttp();
            // Member Functions ----------------------------------------------->
            void run();
        private:
            // -- Used for executing the client threads.
            static void threadHandleClient(WebHttp *web, Client *client, int threadId);
            // -- Kills all the threads.
            void disposeThreads();
            // -- Used for handling startup errors.
            void startupFailure(string message);
        public:
            // Member Functions - Accessors ----------------------------------->
            inline int activeThreads() { return threadsAvailable; }
            inline int getPort() { return port; }
            inline int getSocketsBacklog() { return socketsBacklog; }
            inline int getThreadsMax() { return threadsMax; }
        public:
            string getTitle() { return SERVICETITLE_WEBHTTP; }
        };
    }
}

#endif	/* WEBHTTP_H */

