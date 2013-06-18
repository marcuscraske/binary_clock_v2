
#ifndef RELAYBOARD_H
#define	RELAYBOARD_H

// Includes ------------------------------------------------------------------->
#include "Definitions.h"

#include <iostream>
using std::cout;
using std::endl;
using std::endl;

#include <map>
using std::map;

#include <vector>
using std::vector;

#include <string>
using std::string;

#include <mutex>
using std::mutex;

#include "IService.h"
using BC::Services::IService;

#include "ServiceController.h"
using BC::ServiceController;

#include "Configurator.h"
using BC::Services::Configurator;

#include "RelayCondition.h"
using BC::Services::RelayCondition;

#include "RelayConditionFunctions.h"
using BC::Services::RelayConditionFunctions;

#include "Relay.h"
namespace BC
{
    namespace Services
    {
        class Relay;
    }
}
using BC::Services::Relay;

// Definitions ---------------------------------------------------------------->
#define RELAYBOARD_LOGIC_INTERVAL       1000    // The delay in m/s between checking the logic for sockets.
#define RELAYBOARD_DEFAULT_SOCKETS      6       // The default number of relay-board relays.

namespace BC
{
    namespace Services
    {
        class RelayBoard : public IService
        {
            friend Relay;
        private:
            // Fields --------------------------------------------------------->
            vector<Relay*>                                                      relayPins;              // Holds all of the relays belonging to this relay board; each index corresponds to the virtual socket.
            mutex                                                               lmutex;                 // The mutex used for safely modifying the collection of relays.
            map<string, RelayCondition::ConditionFunction>                   conditionFuncs;         // All of the condition fuction mappings; name -> function. Should only be modified by prerun.
        public:
            // Member Functions - Constructors -------------------------------->
            RelayBoard(ServiceController *controller) : IService(controller)
            {}
            // Member Functions ----------------------------------------------->
            void prerun();
            void run();
            void logic();
            // Member Functions - Accessors
            inline int getTotalRelays() { return relayPins.size(); }
            inline Relay* getRelay(int index) { return relayPins.at(index); }
            string getTitle() { return "Relay Board Controller"; }
        };
    }
}

#endif	/* RELAYBOARD_H */

