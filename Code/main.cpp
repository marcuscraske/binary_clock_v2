#include <iostream>
#include <signal.h>
#include "ServiceController.h"

#include "HttpHandler.h"
#include "Display.h"
#include "Sensors.h"
#include "Alarm.h"
#include "WebHttp.h"
#include "Configurator.h"
#include "CountryLookup.h"
#include "RelayBoard.h"
using namespace BC::Services;

#include "ClientSocket.h"
#include <fstream>
using std::ofstream;
using BC::Web::Core::ClientSocket;

#include "Utils.h"

#define HARDWARE_VIRTUAL_MODE   false    // Indicates if hardware interaction should be disabled for
                                        // virtual-mode for testing purposes.

using BC::ServiceController;
using BC::Utils;
using std::cout;
using std::endl;

ServiceController *controller = 0;

// Useful tutorial: http://www.yolinux.com/TUTORIALS/C++Signals.html
// This function catches termination signals to shut-down the program gracefully
void signalHandler(int signal)
{
    cout << "Process exit signal caught, terminating controller..." << endl;
    if(controller != 0 && controller->isRunning())
        controller->stop();
    delete controller;
    cout << "Good-bye!" << endl;
    exit(signal);
}

int main(int argc, char** argv)
{
    // Add signal handlers
    signal(SIGINT, signalHandler);      // Interrupt from keyboard signal
    signal(SIGTERM, signalHandler);     // Termination signal

    // Create service controller
    controller = new ServiceController(!HARDWARE_VIRTUAL_MODE);
    // Add services
    // -- Alarm
    controller->serviceAdd(new Alarm(controller));
    // -- Configurator
    controller->serviceAdd(new Configurator(controller));
    // -- Display
    controller->serviceAdd(new Display(controller));
    // -- Sensors
    controller->serviceAdd(new Sensors(controller));
    // -- Web HTTP
    HttpHandler *handler = new HttpHandler();
    controller->serviceAdd(new WebHttp(controller, handler));
    // -- Country Lookup Service
    controller->serviceAdd(new CountryLookup(controller));
    // -- Relay Board
    controller->serviceAdd(new RelayBoard(controller));
    // Start the service controller
    controller->start();
    // Wait for the controller to terminate
    controller->waitForShutdown();
    // Stop all services
    controller->stop();
    // Dispose resources
    delete controller;
    // Good-bye - return signal to say we've shutdown successfully!
    std::cout << "Shutdown successful." << std::endl;
    return 0;
}
