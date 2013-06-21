#ifndef SERVICECONTROLLER_H
#define	SERVICECONTROLLER_H

#include <mutex>
#include <map>
#include <thread>
#include <condition_variable>

#include "IService.h"

#include "RaspberryPi.h"
using BC::Hardware::RaspberryPi;

using std::mutex;
using std::unique_lock;
using std::thread;
using std::condition_variable;
using std::map;

// Forward declarations
namespace BC
{
    namespace Services
    {
        class IService;
    }
    namespace Web
    {
        namespace Http
        {
            class HttpHandler;
        }
        namespace Core
        {
            class IClientHandler;
        }
    }
}
using BC::Services::IService;
using BC::Web::Http::HttpHandler;

namespace BC
{
    // The class responsible for adding, removing, starting and stopping
    // services; this also acts as a bridge for inter-service communication
    // (ISC).
    class ServiceController
    {
    public:
        // Enums -------------------------------------------------------------->
        enum GPIO_Mode
        {
            Input,
            Output
        };
        enum RestartType
        {
            Start,
            Stop,
            Restart
        };
    private:
        // Fields ------------------------------------------------------------->
        bool                            flagRunning,            // Indicates if the controller's services are running.
                                        flagRestartServices;    // Indicates if the controller needs to restart the services.
        mutex                           mutexController;        // Used for thread-safe operations of internal operations on data.
        map<string, IService*>          services;               // Holds a collection of services belonging to the controller.
        bool                            hardwareEnabled;        // Indicates if we can communicate with hardware.
        thread                          threadWorker;           // Used for misc tasks such as restarting services.
        condition_variable              cvWorker;               // Used to wait until flags change to process background tasks; also
                                                                // used by the waitForShutdown() function.
        mutex                           mutexWorker;            // The mutex for synchronising the worker locks.
        RaspberryPi                     *pi;                    // Used for interfacing with the Raspberry Pi GPIO pins.
        map<string, RestartType>        flagRestartService;     // used for starting/stopping/restarting specific services.
    public:
        // Constructors ------------------------------------------------------->
        ServiceController(bool hardwareEnabled);
        ServiceController() : ServiceController(true) {}
        // Destructors -------------------------------------------------------->
        ~ServiceController();
        // Member Functions --------------------------------------------------->
        // -- Creates and starts the services of the controller.
        void start();
        // -- Stops the services of the controller.
        void stop();
        // -- Waits for the controller to shutdown.
        void waitForShutdown();
        // -- Restarts all of the services.
        void restartServices();
        // -- Performs a specific start/stop/restart of a service.
        void restartService(string title, RestartType type);
    private:
        static void backgroundWorker(ServiceController *controller);
        // Stops the services; no thread-safe protection.
        void stopServices();
    public:
        // Member Functions - Mutators ---------------------------------------->
        inline bool serviceAdd(IService *service)
        {
            unique_lock<mutex> lock(mutexController);
            if(flagRunning)
            {
                cerr << "Cannot add a service when the controller is operational!" << endl;
                return false;
            }
            services[service->getTitle()] = service;
            return true;
        }
        inline bool serviceRemove(IService *service)
        {
            unique_lock<mutex> lock(mutexController);
            if(flagRunning)
            {
                cerr << "Cannot remove a service when the controller is operational!" << endl;
                return false;
            }
            // Find the service
            map<string, IService*>::iterator it = services.begin();
            while(it != services.end() && (*it).second != service)
                it++;
            // Remove it
            if(it != services.end())
            {
                services.erase(it);
                return true;
            }
            else
            {
                cerr << "Could not remove service with pointer '" << service << "'; not found in the service collection!" << endl;
                return false;
            }
        }
        inline bool serviceRemove(string name)
        {
            unique_lock<mutex> lock(mutexController);
            if(flagRunning)
            {
                cerr << "Cannot remove a service when the controller is operational!" << endl;
                return false;
            }
            map<string, IService*>::iterator it = services.find(name);
            if(it == services.end())
            {
                cerr << "Failed to remove service '" << name << "' - not found!" << endl;
                return false;
            }
            else
            {
                services.erase(it);
                return true;
            }
        }
        // Member Functions - Accessors --------------------------------------->
        inline bool isRunning() { return flagRunning; }
        inline bool isHardwareEnabled() { return hardwareEnabled; }
    private:
        inline IService* _getServiceByName(string name)
        {
            if(!flagRunning)
            {
                cerr << "Attempted to get service (by name) when controller is not running!" << endl;
                return 0;
            }
            map<string, IService*>::iterator it = services.find(name);
            return it == services.end() ? 0 : (*it).second;
        }
    public:
        // --Fetches a service by its title name; returns 0/null-pointer if the
        // -- service could not be found.
        inline IService* getServiceByName(string name)
        {
            unique_lock<mutex> lock(mutexController);
            return _getServiceByName(name);
        }
        // -- Same as getServices without a lock on the controller's mutex;
        // -- thus not thread-safe.
        inline map<string, IService*>getServicesUnsafe()
        {
            if(!flagRunning)
            {
                cerr << "Attempted to get services collection when controller is not running!" << endl;
                return map<string, IService*>();
            }
            else
                return services;
        }
        // -- Retrieves all of the services belonging to this controller. 
        // -- Thread-safe.
        inline map<string, IService*> getServices()
        {
            unique_lock<mutex> lock(mutexController);
            return getServicesUnsafe();
        }
        inline mutex *getMutex()
        {
            return &mutexController;
        }
        // Sets the shutdown flag to true; the thread responsible for the
        // creation of this object/controller should check this flag. When the
        // flag becomes false, it should invoke stop.
        inline void requestShutdown()
        {
            flagRunning = false;
            // Wake and join background worker
            cvWorker.notify_all();
            if(threadWorker.joinable())
                threadWorker.join();
        }
        // Member Functions - GPIO *********************************************
        ////////////////////////////////////////////////////////////////////////
        // Sets the GPIO mode of a pin (input/output).
        ////////////////////////////////////////////////////////////////////////
        inline void gpioSetPinMode(int pin, GPIO_Mode mode)
        {
            if(pi != 0)
            {
                switch(mode)
                {
                    case GPIO_Mode::Input:
                        pi->gpioMode(pin, RaspberryPi::PinMode::Input);
                        break;
                    case GPIO_Mode::Output:
                        pi->gpioMode(pin, RaspberryPi::PinMode::Output);
                        break;
                }
            }
        }
        ////////////////////////////////////////////////////////////////////////
        // Reads the input signal of a GPIO pin (on/off, high/low).
        ////////////////////////////////////////////////////////////////////////
        inline bool gpioRead(int pin)
        {
            if(pi != 0)         return pi->gpioRead(pin);
            else                return false;
        }
        ////////////////////////////////////////////////////////////////////////
        // Writes the output signal of a GPIO pin (on/off, high/low).
        ////////////////////////////////////////////////////////////////////////
        inline bool gpioWrite(int pin, bool state)
        {
            if(pi != 0) pi->gpioWrite(pin, state);
        }
    };
}

#endif	/* SERVICECONTROLLER_H */

