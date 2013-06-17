
#include "WebHttp.h"
#include "Configurator.h"

using BC::Services::Configurator;

namespace BC
{
    namespace Services
    {
        WebHttp::~WebHttp()
        {
            delete clientHandler;
        }
        void WebHttp::startupFailure(string message)
        {
            cerr << getTitle() << ": " << message << endl;
            shutdown = true;
            delete[] threads;
            delete[] threadsDead;
        }
        void WebHttp::run()
        {
            unique_lock<mutex> cl(mutexService);
            // Load configuration
            {
                Configurator *c = static_cast<Configurator*>(controller->getServiceByName(SERVICETITLE_CONFIGURATOR));
                port = c->get(CONFIG__WEBHTTP__PORT, PORT_MIN, PORT_MAX, WEBHTTP_DEFAULT_PORT);
                socketsBacklog = c->get(CONFIG__WEBHTTP__SOCKETS_BACKLOG, 1, 99999, WEBHTTP_DEFAULT_SOCKETS_BACKLOG);
                threadsMax = threadsAvailable = c->get(CONFIG__WEBHTTP__THREADS_MAX, 1, 99999, WEBHTTP_DEFAULT_THREADS_MAX);
            }
            cout << getTitle() << ": starting web-service; allocating thread pool..." << endl;
            // Setup thread-pool
            threads = new thread*[threadsMax];
            threadsDead = new bool[threadsMax];
            // Set each thread to a null-pointer
            for(int i = 0; i < threadsMax; i++)
            {
                threads[i] = 0;
                threadsDead[i] = true;
            }
            // Attempt to create a new socket
            socketListen = socket(AF_INET, SOCK_STREAM, 0);
            if(socketListen < 0)
            {
                startupFailure("Error: failed to create new socket!");
                return;
            }
            cout << getTitle() << ": successfully acquired socket..." << endl;
            // Setup the listener socket details
            struct sockaddr_in addrServer;
            addrServer.sin_family = AF_INET;
            addrServer.sin_addr.s_addr = INADDR_ANY;
            addrServer.sin_port = htons(port);
            // Attempt to bind the socket
            // -- Keep retrying if we fail until we acquire the socket.
            while(!shutdown && bind(socketListen, (struct sockaddr *) &addrServer, sizeof(addrServer)) < 0)
            {
                startupFailure("Error: failed to create new socket!");
                Utils::conditionVariableSleep(cl, cvService, WEBHTTP_SOCKET_POLLRATE_MS);
            }
            // In-case we have failed to get a socket, check if the shutdown condition was fulfilled - if so, abort.
            if(shutdown)
            {
                close(socketListen);
                return;
            }
            cout << getTitle() << ": successfully binded web-server to port '" << port << "'!" << std::endl;
            // Start listening
            listen(socketListen, socketsBacklog);
            // Inform the client-handler we're ready
            clientHandler->serviceStart(this);
            // Wait for connections
            Client *client;
            socklen_t addrClientLength;
            int threadId;
            // Set the listen socket to non-block mode too
            // -- F_SETFL = set descriptor flags
            fcntl(socketListen, F_SETFL, O_NONBLOCK);
            while(!shutdown)
            {
                // Grab a thread to handle the request
                std::unique_lock<std::mutex> mutex(threadsMutex);
                // -- Wait for a thread to be released
                while(threadsAvailable == 0)
                    threadsMutexCondition.wait(mutex);
                // -- Delete any possible threads awaiting deletion
                disposeThreads();
                // -- Find the next thread available
                threadId = -1;
                for(int i = 0; i < threadsMax; i++)
                    if(threads[i] == 0)
                    {
                        threadId = i;
                        break;
                    }
                threadsDead[threadId] = false;
                threadsAvailable--;
                threadsMutex.unlock();
                // Accept the client
                client = new Client;
                addrClientLength = sizeof(client->addrClient);
                // Wait for a valid client
                // -- http://manpages.courier-mta.org/htmlman2/accept.2.html
                // Loop until we get a connection
                while(!shutdown && (client->socket = accept(socketListen, (struct sockaddr *) &(client->addrClient), &addrClientLength)) < 0)
                    usleep(500); // This seems horrible and wastes a lot of CPU cycles; needs improving! We can't just block until socket due to shutdown variable.
                // Set the client info
                inet_ntop(AF_INET, &client->addrClient.sin_addr, client->ipAddress, sizeof client->ipAddress); // Client's IP
                client->threadId = threadId;
                // Check the socket is valid
                if(client->socket < 0)
                {
                    close(client->socket); // Ensure the socket is disposed, despite failure.
                    if(controller->isRunning())
                        cerr << getTitle() << ": error: failed to accept socket of client!" << std::endl;
                    delete client;
                }
                else
                {
                    // Create a client thread to handle the request
                    thread *th = new thread(WebHttp::threadHandleClient, this, client, threadId);
                    threads[threadId] = th;
                }
            }
            // Inform the client-handler we've shutdown
            clientHandler->serviceEnd(this);
            // Dispose listener socket
            close(socketListen);
            // Dispose thread-pool
            disposeThreads();
            delete[] threads;
            delete[] threadsDead;
        }
        void WebHttp::disposeThreads()
        {
            // We delete any pointers which are not null but set as available
            for(int i = 0; i < threadsMax; i++)
                if(threads[i] != 0 && threadsDead[i])
                {
                    // Join the thread to ensure execution has finished and then delete it; reset the pointer to zero for reallocation
                    if(threads[i]->joinable())
                        threads[i]->join();
                    delete threads[i];
                    threads[i] = 0;
                }
        }
        void WebHttp::threadHandleClient(WebHttp *web, Client *client, int threadId)
        {
            // Push the client to the handler for further actions
            web->clientHandler->handleClient(web, client);
            // Dispose socket and client
            close(client->socket);
            delete client;
            // Change the status of the thread to available and self-terminate
            std::lock_guard<std::mutex> lock(web->threadsMutex);
            web->threadsDead[threadId] = true;
            web->threadsAvailable++;
            // Notify/wake any other threads waiting to grab a new thread, due to no threads being available
            web->threadsMutexCondition.notify_one();
        }
    }
}