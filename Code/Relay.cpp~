
#include "Relay.h"

namespace BC
{
    namespace Services
    {
        Relay::Relay(RelayBoard *board, int index) : board(board), index(index), online(false), lastSwitched(0LL)
        {
            Configurator *c = static_cast<Configurator*>(board->controller->getServiceByName(SERVICETITLE_CONFIGURATOR));
            string t = Utils::intToString(index);
            // Parse and setup GPIO pin
            gpioPin = c->get("relayboard.socket_" + t + "_gpio_pin", -1);
            if(gpioPin >= 0)
                board->controller->gpioSetPinMode(gpioPin, ServiceController::GPIO_Mode::Output);
            // Parse the label
            label = c->get("relayboard.socket_" + t + "_label", "Untitled Relay #" + t);
            // Parse conditions
            parseConditions(c->get("relayboard.socket_" + t + "_conditions_on", ""), conditionsOn);
            parseConditions(c->get("relayboard.socket_" + t + "_conditions_off", ""), conditionsOff);
            cout << "Relay ~ " << index << ": setup on GPIO pin '" << gpioPin << "'; on conditions: '" << conditionsOn.size() << "', off conditions: '" << conditionsOff.size() << "'." << endl;
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
            vector<string> opt;
            while(ss.good())
            {
                getline(ss, line, '\n');
                opt = Utils::split(line, ',');
                if(opt.size() >= 1 && opt.size() <= 3)
                {
                    RelayCondition::ConditionFunction func = board->conditionFuncs[opt.at(0)];
                    if(func != 0)
                    {
                        RelayCondition rc(opt.at(0), func, opt.size() >= 2 ? opt.at(1) : "", opt.size() == 3 ? opt.at(2) : "");
                        rc.uniqueID = uniqueIDCounter++;
                        list.push_back(rc);
                    }
                }
                else if(opt.size() > 0)
                    cerr << "Relay ~ " << index << ": invalid configuration line '" << line << "' whilst parsing (size: " << opt.size() << ")!" << endl;
            }
        }
        void Relay::saveList(Configurator* c, string key, vector<RelayCondition> &list)
        {
            stringstream ss;
            for(vector<RelayCondition>::iterator it = list.begin(); it != list.end(); it++)
            {
                RelayCondition cond = *it;
                ss << cond.conditionName << "," << cond.param1 << "," << cond.param2 << '\n';
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
            // Save to configurator
            Configurator *c = static_cast<Configurator*>(board->controller->getServiceByName(SERVICETITLE_CONFIGURATOR));
            c->set("relayboard.socket_" + Utils::intToString(index) + "_gpio_pin", Utils::intToString(gpioPin));
        }
        void Relay::changeLabel(string newLabel)
        {
            unique_lock<mutex> lock(lmutex);
            label = newLabel;
            // Save to configurator
            Configurator *c = static_cast<Configurator*>(board->controller->getServiceByName(SERVICETITLE_CONFIGURATOR));
            c->set("relayboard.socket_" + Utils::intToString(index) + "_label", label);
        }
        void Relay::addConditionOn(RelayCondition cond)
        {
            unique_lock<mutex> lock(lmutex);
            cond.uniqueID = uniqueIDCounter++;
            conditionsOn.push_back(cond);
            // Save
            Configurator *c = static_cast<Configurator*>(board->controller->getServiceByName(SERVICETITLE_CONFIGURATOR));
            saveList(c, "relayboard.socket_" + Utils::intToString(index) + "_conditions_on", conditionsOn);
        }
        void Relay::addConditionOff(RelayCondition cond)
        {
            unique_lock<mutex> lock(lmutex);
            cond.uniqueID = uniqueIDCounter++;
            conditionsOff.push_back(cond);
            // Save
            Configurator *c = static_cast<Configurator*>(board->controller->getServiceByName(SERVICETITLE_CONFIGURATOR));
            saveList(c, "relayboard.socket_" + Utils::intToString(index) + "_conditions_on", conditionsOff);
        }
        bool Relay::removeCondition(int uniqueID)
        {
            unique_lock<mutex> lock(lmutex);
            // Search on-list
            for(vector<RelayCondition>::iterator it = conditionsOn.begin(); it != conditionsOn.end(); it++)
                if((*it).uniqueID == uniqueID)
                {
                    conditionsOn.erase(it);
                    save();
                    return true;
                }
            // Search off-list
            for(vector<RelayCondition>::iterator it = conditionsOff.begin(); it != conditionsOff.end(); it++)
                if((*it).uniqueID == uniqueID)
                {
                    conditionsOff.erase(it);
                    save();
                    return true;
                }
            return false;
        }
    }
}

int BC::Services::Relay::uniqueIDCounter = 0;
