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
                    cout << "Stopping '" << s->getTitle() << "'..." << endl;
                    s->stop();
                    cout << "Starting '" << s->getTitle() << "'..." << endl;
                    s->start();
                    cout << "Restarted '" << s->getTitle() << "'!" << endl;
                }
                cout << "Restarted services complete!" << endl;
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
}