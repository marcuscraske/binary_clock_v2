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
 *      File:           IService.h
 *      Path:           BC/Services/IService.h
 * 
 *      Change-Log:
 *                      2013-06-25      Added header.
 * 
 * *****************************************************************************
 * The base-class used by all services; this is almost like an interface, except
 * it contains some implemented code.
 * *****************************************************************************
 */
#ifndef ISERVICE_H
#define	ISERVICE_H

#include <thread>
#include <pthread.h>
using std::thread;

#include <mutex>
using std::mutex;
using std::unique_lock;

#include <sys/time.h>

#include <string>
using std::string;

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <condition_variable>
using std::condition_variable;

// Forward declarations
#include "ServiceController.h"
namespace BC
{
    class ServiceController;
}
using BC::ServiceController;

namespace BC
{
    namespace Services
    {
        class IService
        {
            // Fields --------------------------------------------------------->
        private:
            thread                      *sThread;               // The thread used by the service for execution.
        protected:
            bool                        shutdown;               // Shutdown flag.
            ServiceController           *const controller;      // Controller of service.
            mutex                       mutexThread;            // Mutex used for thread safety of critical service variables i.e. shutdown flag.
            condition_variable          cvService;              // Used for sleeping until a service event occurs.
            mutex                       mutexService;           // The mutex used for sleeping until a service event occurs.
        public:
            // Constructors --------------------------------------------------->
            IService(ServiceController * const controller) : controller(controller), shutdown(true) {}
            // Member Functions ----------------------------------------------->
            // Retrieves the name of the service.
            virtual string getTitle() { return "Undefined service"; }
            // Invoked to start the service.
            void start()
            {
                unique_lock<mutex> lock(mutexThread);
                if(!shutdown)
                {
                    cerr << "Cannot start service ('" << getTitle() << "'); already running!" << endl;
                    return;
                }
                shutdown = false;
                sThread = new thread(runThread, this);
            }
            // Invoked to stop the service by setting the shutdown flag
            // and joining the thread of the service to the invoking thread.
            // This causes the entire application to hang, assuming the primary
            // thread is calling, until the service is finished.
            void stop()
            {
                unique_lock<mutex> lock(mutexThread);
                if(shutdown)
                    return;
                shutdown = true;
                // Notify the condition variable we're shutting down
                cout << getTitle() + ": notifying service..." << endl;
                cvService.notify_all();
                // Join and dispose the thread
                cout << getTitle() + ": joined thread..." << endl;
                sThread->join();
                delete sThread;
            }
            // Executed before the fun member-function; used for loading
            // any data for other services.
            virtual void prerun() {}
            // The execution function of the service, executed under another
            // thread.
            virtual void run() { }
            // Used for anonymous inter-service communication.
            virtual void anonComm() { }
            virtual string anonCommStr(string data) {}
            virtual int anonCommInt(string data) {}
            // Member Functions - Accessors ----------------------------------->
            // Retrieves the mutex used to lock and modify the core operations
            // of the service e.g. starting and shutting down.
            std::mutex *getMutex() { return &mutexThread; }
            // Retrieves the controller, responsible for managing the service.
            inline ServiceController *getController() { return controller; }
            // Indicates if the service is running.
            inline bool isRunning() { return !shutdown; }
            inline void wake() { cvService.notify_all(); }
        private:
            // Since we cannot directly call a member-function for a new thread,
            // this function invokes a member function of a passed service.
            static void runThread(IService *service)
            {
                try
                {
                    service->run();
                }
                catch(std::exception ex)
                {
                    cerr << "CRITICAL SERVICE FAILURE OF '" << service->getTitle() << "'!" << endl;
                    cerr << "Service '" << service->getTitle() << "' has failed; service is now shutdown." << endl;
                }
            }
        };
    }
}

#endif	/* ISERVICE_H */
