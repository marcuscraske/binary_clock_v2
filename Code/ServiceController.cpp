#include "ServiceController.h"

namespace BC
{
    ServiceController::ServiceController(bool hardwareEnabled) : hardwareEnabled(hardwareEnabled), flagRunning(false), flagRestartServices(false)
    {
        if(hardwareEnabled)
        {
            // Setup pi
            pi = new RaspberryPi();
            if(pi->setup())
                cout << "Service Controller: successfully setup Raspberry Pi." << endl;
            else
            {
                cerr << "Service Controller: failed to setup Raspberry Pi!" << endl;
                delete pi;
                pi = 0;
            }
        }
        else
        {
            cerr << "WARNING: running system in virtual hardware mode!" << endl;
            pi = 0;
        }
    }
    ServiceController::~ServiceController()
    {
        unique_lock<mutex> lock(mutexController);
        // Check if to stop the services
        if(flagRunning)
            stopServices();
        // Dispose services
        for(map<string, IService*>::iterator it = services.begin(); it != services.end(); it++)
            delete (*it).second;
        // Dispose pi
        if(pi != 0) delete pi;
    }
    void ServiceController::backgroundWorker(ServiceController *controller)
    {
        while(controller->flagRunning)
        {
            unique_lock<mutex> lockWorker(controller->mutexWorker);
            // Wait until a flag changes
            controller->cvWorker.wait(lockWorker);
            // Grab a mutex lock - we check the flag first to avoid a dead-lock
            if(!controller->flagRunning)    // Arguably this is not thread-safe, but the thread should be shutdown if this is not running - it cannot change to running when this thread is executing!
                break;
            unique_lock<mutex> lock(controller->mutexController);
            // Perform background tasks
            // -- Check if to restart services
            if(controller->flagRestartServices)
            {
                cout << "Controller background worker: restarting services..." << endl;
                IService *s;
                for(map<string, IService*>::iterator it = controller->services.begin(); it != controller->services.end(); it++)
                {
                    s = (*it).second;
                    cout << "Controller background worker: stopping '" << s->getTitle() << "'..." << endl;
                    s->stop();
                    cout << "Controller background worker: starting '" << s->getTitle() << "'..." << endl;
                    s->start();
                    cout << "Controller background worker: restarted '" << s->getTitle() << "'!" << endl;
                }
                cout << "Restarted services complete!" << endl;
            }
            // -- Check if to perform service-specific tasks
            if(controller->flagRestartService.size() > 0)
            {
                IService *s;
                string title;
                for(map<string, RestartType>::iterator it = controller->flagRestartService.begin(); it != controller->flagRestartService.end(); it++)
                {
                    title = (*it).first;
                    s = controller->_getServiceByName(title);
                    if(s == 0)
                        cout << "Controller background worker: failed to find service '" << title << "' for specific task!" << endl;
                    else
                    {
                        switch((*it).second)
                        {
                            case RestartType::Restart:
                                cout << "Controller background worker: restarting '" << title << "' ~ stopping..." << endl;
                                s->stop();
                                cout << "Controller background worker: restarting '" << title << "' ~ starting..." << endl;
                                s->start();
                                break;
                            case RestartType::Start:
                                cout << "Controller background worker: starting '" << title << "'!" << endl;
                                s->start();
                                break;
                            case RestartType::Stop:
                                cout << "Controller background worker: stopping '" << title << "'!" << endl;
                                s->stop();
                                break;
                        }
                    }
                }
                controller->flagRestartService.clear();
            }
        }
    }
    void ServiceController::waitForShutdown()
    {
        unique_lock<mutex> lockWorker(mutexWorker);
        while(flagRunning)
            cvWorker.wait(lockWorker);
    }
    void ServiceController::restartServices()
    {
        unique_lock<mutex> lock(mutexController);
        // Check the controller is still running
        if(!flagRunning)
            return;
        // Update the flag for the background service
        flagRestartServices = true;
        // Wake background service
        cvWorker.notify_all();
    }
    void ServiceController::restartService(string title, RestartType type)
    {
        unique_lock<mutex> lock(mutexController);
        // Check the controller is still running
        if(!flagRunning)
            return;
        // Add the service specific task to the scheduler
        // -- The great thing with a map is that the same service with a new task will replace the old!
        flagRestartService[title] = type;
        // Wake background service
        cvWorker.notify_all();
    }
    void ServiceController::start()
    {
        unique_lock<mutex> lock(mutexController);
        // Execute pre-run function of each service
        for(map<string, IService*>::iterator it = services.begin(); it != services.end(); it++)
            (*it).second->prerun();
        // Start every service (which will execute each service in a separate thread)
        for(map<string, IService*>::iterator it = services.begin(); it != services.end(); it++)
            (*it).second->start();
        // Update running flag
        flagRunning = true;
        // Launch background worker
        threadWorker = thread(backgroundWorker, this);
    }
    void ServiceController::stopServices()
    {
        // Update status
        flagRunning = false;
        // Terminate services
        IService *s;
        for(map<string, IService*>::iterator it = services.begin(); it != services.end(); it++)
        {
            s = (*it).second;
            cout << "Stopping '" << s->getTitle() << "'..." << endl;
            s->stop();
            cout << "Stopped '" << s->getTitle() << "'." << endl;
        }
        // Kill background worker and waitForShutdown() function
        cout << "Terminating background worker..." << endl;
        cvWorker.notify_all();
        cout << "Notified; joining thread..." << endl;
        if(threadWorker.joinable())
            threadWorker.join();
        cout << "Terminated background worker." << endl;
    }
    void ServiceController::stop()
    {
        unique_lock<mutex> lock(mutexController);
        // Stop every service
        stopServices();
    }
    bool ServiceController::serviceAdd(IService *service)
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
    bool ServiceController::serviceRemove(IService *service)
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
    bool ServiceController::serviceRemove(string name)
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
}