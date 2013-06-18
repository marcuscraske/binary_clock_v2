
#include "Relay.h"

namespace BC
{
    namespace Services
    {
        Relay::Relay(RelayBoard *board, int index) : board(board), index(index), online(false), lastSwitched(0LL)
        {
            Configurator *c = static_cast<Configurator*>(board->controller->getServiceByName(SERVICETITLE_CONFIGURATOR));
            // Parse and setup GPIO pin
            gpioPin = c->get("relayboard.socket_" + Utils::intToString(index) + "_gpio_pin", -1);
            if(gpioPin >= 0)
                board->controller->gpioSetPinMode(gpioPin, ServiceController::GPIO_Mode::Output);
            // Parse conditions
            string t = Utils::intToString(index);
            parseConditions(c->get("relayboard.socket_" + t + "_conditions_on", ""), conditionsOn);
            parseConditions(c->get("relayboard.socket_" + t + "_conditions_off", ""), conditionsOff);
            cout << "Relay ~ " << index << ": setup on GPIO pin '" << gpioPin << "'." << endl;
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
                    cerr << "Relay ~ " << index << ": invalid configuration line '" << line << "' whilst parsing!" << endl;
            }
        }
        void Relay::saveList(Configurator* c, string key, vector<RelayCondition> &list)
        {
            stringstream ss;
            for(vector<RelayCondition>::iterator it = list.begin(); it != list.end(); it++)
            {
                RelayCondition cond = *it;
                ss << cond.conditionName << "<" << cond.param1 << "," << cond.param2 << endl;
            }
            c->set(key, ss.str());
        }
        void Relay::save()
        {
            Configurator *c = static_cast<Configurator*>(board->controller->getServiceByName(SERVICETITLE_CONFIGURATOR));
            if(c != 0)
            {
                string t = Utils::intToString(index);
                saveList(c, "relayboard.socket_" + t + "_conditions_on", conditionsOn);
                saveList(c, "relayboard.socket_" + t + "_conditions_off", conditionsOff);
            }
        }
        void Relay::changeGPIOPin(int newPin)
        {
            unique_lock<mutex> lock(lmutex);
            // Set the old pin off for safety
            if(gpioPin >= 0)
                board->controller->gpioWrite(gpioPin, false);
            // Setup the new pin
            gpioPin = newPin;
            if(gpioPin >= 0)
                board->controller->gpioSetPinMode(gpioPin, ServiceController::GPIO_Mode::Output);
        }
    }
}
