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
 *      File:           RelayBoard.h
 *      Path:           BC/Services/RelayBoard.h
 * 
 *      Change-Log:
 *                      2013-06-25      Added header.
 * 
 * *****************************************************************************
 * The relay-board service, used to operate the hardware relay-board on the
 * binary clock. This service runs the logic to check any conditions of
 * individual relays and handles the on-board relay board.
 * *****************************************************************************
 */
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

namespace BC
{
    namespace Services
    {
        class RelayBoard : public IService
        {
            friend Relay;
        private:
            // Fields --------------------------------------------------------->
            vector<Relay*>                                      relayPins;              // Holds all of the relays belonging to this relay board; each index corresponds to the virtual socket.
            mutex                                               lmutex;                 // The mutex used for safely modifying the collection of relays.
            map<string, RelayCondition::ConditionFunction>      conditionFuncs;         // All of the condition fuction mappings; name -> function. Should only be modified by prerun.
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
            // Not thread-safe!
            Relay* getRelay(int index)
            {
                if(index >= relayPins.size())
                    return 0;
                else
                    return relayPins.at(index);
            }
            string getTitle() { return SERVICETITLE_RELAYBOARD; }
            inline mutex* getCollectionMutex() { return &lmutex; }
            map<string, RelayCondition::ConditionFunction> getConditionFunctions() { return conditionFuncs; }
            RelayCondition::ConditionFunction getConditionFunction(string name)
            {
                map<string, RelayCondition::ConditionFunction>::iterator it = conditionFuncs.find(name);
                if(it == conditionFuncs.end())
                    return 0;
                else
                    return (*it).second;
            }
        };
    }
}

#endif	/* RELAYBOARD_H */

