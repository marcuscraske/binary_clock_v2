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
 *      Author(s):      limpygnome (Marcus Craske)      limpygnome@gmail.com
 * 
 *      License:        Creative Commons Attribution-ShareAlike 3.0 Unported
 *                      http://creativecommons.org/licenses/by-sa/3.0/
 * 
 *      File:           Relay.h
 *      Path:           BC/Hardware/Relay.h
 * 
 *      Change-Log:
 *                      2013-06-13      Created.
 * 
 * *****************************************************************************
 * Used to specifically control a relay on a relay-board, with the relay
 * electronically-tied to a GPIO pin.
 * *****************************************************************************
 */

#ifndef RELAY_H
#define	RELAY_H

// Includes ------------------------------------------------------------------->
#include <iostream>
using std::cerr;
using std::endl;

#include <string>
using std::string;

#include <sstream>
using std::stringstream;

#include <vector>
using std::vector;

#include <mutex>
using std::mutex;
using std::unique_lock;

#include "Utils.h"
using BC::Utils;

#include "RelayCondition.h"
using BC::Services::RelayCondition;

#include "RelayBoard.h"
#include "Configurator.h"

// Forward declarations
namespace BC
{
    namespace Services
    {
        class RelayBoard;
    }
}
using BC::Services::RelayBoard;

// Definitions ---------------------------------------------------------------->
////////////////////////////////////////////////////////////////////////////////
// The minimum delay in m/s between switching the relay on/off for safety
// reasons.
////////////////////////////////////////////////////////////////////////////////
#define RELAY_MINIMUM_SWITCH_DELAY             5000

namespace BC
{
    namespace Services
    {
        class Relay
        {
        private:
            // Static Fields
            static int uniqueIDCounter;                                // Used for assigning ID's for relay conditions, as a temp. point of reference.
            // Fields --------------------------------------------------------->
            RelayBoard                  *board;                 // The parent of the relay.
            int                         gpioPin;                // The GPIO pin.
            bool                        online;                 // Indicates if the relay is on (power flowing) or off (circuit broken).
            long long                   lastSwitched;           // Indicates when the relay was last toggled.
            vector<RelayCondition>      conditionsOn,           // List of logical OR functions for switching the relay on.
                                        conditionsOff;          // List of logical OR functions for switching the relay off.
            mutex                       lmutex;                 // Used for thread-safe operations.
            int                         index;                  // The index of the relay.
            string                      label;                  // The label of the relay socket; used for humans.
        public:
            // Member Functions - Constructors -------------------------------->
            Relay(RelayBoard *board, int index);
        private:
            // Member Functions ----------------------------------------------->
            ////////////////////////////////////////////////////////////////////
            // Sets the state of the relay. Not thread-safe.
            ////////////////////////////////////////////////////////////////////
            void _set(bool state);
            ////////////////////////////////////////////////////////////////////
            // Toggles the relay on/off. Not thread-safe.
            ////////////////////////////////////////////////////////////////////
            inline void _toggle()
            {
                _set(!online);
            }
            void parseConditions(string config, vector<RelayCondition> &list);
        public:
            void set(bool state)
            {
                unique_lock<mutex> lock(lmutex);
                _set(state);
            }
            void toggle()
            {
                unique_lock<mutex> lock(lmutex);
                _toggle();
            }
            ////////////////////////////////////////////////////////////////////
            // Check all the conditions to see if we need to toggle the power
            // on/off. Thread-safe.
            ////////////////////////////////////////////////////////////////////
            void logic();
            // Member Functions - Accessors ----------------------------------->
            ////////////////////////////////////////////////////////////////////
            // Indicates if this relay is enabled (circuit flowing) or disabled
            // (circuit broken).
            ////////////////////////////////////////////////////////////////////
            inline bool isEnabled() { return online; }
            inline int getGPIOPin() { return gpioPin; }
            inline vector<RelayCondition> getConditionsForOn()
            {
                return conditionsOn;
            }
            inline vector<RelayCondition> getConditionsForOff()
            {
                return conditionsOff;
            }
        private:
            void saveList(Configurator* c, string key, vector<RelayCondition> &list);
            void save();
        public:
            void addConditionOn(RelayCondition cond);
            void addConditionOff(RelayCondition cond);
            bool removeCondition(int uniqueID);
            void changeGPIOPin(int newPin);
            void changeLabel(string newLabel);
            inline mutex *getMutex() { return &lmutex; }
            inline string getLabel() { return label; }
            inline int getIndex() { return index; }
            inline bool getOnline() { return online; }
        };
    }
}

#endif	/* RELAY_H */

