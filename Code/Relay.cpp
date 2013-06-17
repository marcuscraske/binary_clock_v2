
#include "Relay.h"

namespace BC
{
    namespace Services
    {
        Relay::Relay(RelayBoard *board, int gpioPin) : board(board), gpioPin(gpioPin), online(false), lastSwitched(0LL)
        {
            _set(false);        // Set the initial state to off.
        }
        void Relay::_set(bool state)
        {
            online = state;
            // Toggle the pin (if it's valid -may be unset in config)
            if(gpioPin >= 0)
                board->controller->gpioWrite(gpioPin, state);
            // Update the time for safety
            lastSwitched = Utils::getEpochTimeMs();
        }
        void Relay::logic()
        {
            unique_lock<mutex> lock(lmutex);
            // Safety to avoid rapid switching on and off (which could cause damagae/safety issues/possibly even a fire)
            if(Utils::getEpochTimeMs() - lastSwitched <= RELAY_MINIMUM_SWITCH_DELAY)
                return;
            // Test each condition
            vector<RelayCondition> *cond = online ? &conditionsOff : &conditionsOn;
            bool result;
            for(vector<RelayCondition>::iterator it=cond->begin(); it != cond->end() && !result; it++)
            {
                if(result = (*it).test(board->controller))
                    _toggle();
            }
        }
        void Relay::parseConditions(string config, vector<RelayCondition> &list)
        {
            unique_lock<mutex> lock(lmutex);
            stringstream ss;
            ss << config;
            // Format:
            // <condition name,param1,param2>,
            // ...
            string line;
            while(ss.good())
            {
                getline(ss, line, '\n');
                vector<string> opt = Utils::split(config, ',');
                if(opt.size() == 3)
                {
                    RelayCondition::ConditionFunction func = board->conditionFuncs[opt.at(0)];
                    if(func != 0)
                        list.push_back(RelayCondition(opt.at(0), func, opt.at(1), opt.at(2)));
                }
                else if(opt.size() > 0)
                    cerr << "Relay: invalid configuration line '" << line << "' whilst parsing!" << endl;
            }
        }
    }
}
