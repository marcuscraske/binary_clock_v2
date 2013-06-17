
#include "RelayBoard.h"
#include "Configurator.h"

namespace BC
{
    namespace Services
    {
        void RelayBoard::prerun()
        {
            // Load condition function mappings
            conditionFuncs["temperature"] = RelayConditionFunctions::temperature;
            conditionFuncs["light"] = RelayConditionFunctions::light;
            conditionFuncs["time_of_day"] = RelayConditionFunctions::timeOfDay;
        }
        void RelayBoard::run()
        {
            // Add virtual sockets
            {
                Configurator *c = static_cast<Configurator*>(controller->getServiceByName(SERVICETITLE_CONFIGURATOR));
                int totalSocks = c->get(CONFIG__RELAYBOARD_TOTAL_SOCKETS, RELAYBOARD_DEFAULT_SOCKETS);
                Relay *r;
                string t, t2;
                for(int i = 0; i < totalSocks; i++)
                {
                    t = Utils::intToString(i);
                    // Add relay socket
                    r = addRelay(c->get("socket_" + t + "_gpio_pin", -1));
                    // Load condition functions and params
                    // -- Conditions on
                    t2 = c->get("relayboard.socket_" + t + "_conditions_on", "");
                    r->parseConditionsOn(t2);
                    // -- Conditions off
                    t2 = c->get("relayboard.socket_" + t + "_conditions_on", "");
                    r->parseConditionsOff(t2);
                }
                cout << getTitle() << ": added " << totalSocks << " relay sockets!" << endl;
            }
            // Periodically execute logic until shutdown
            unique_lock<mutex> l(mutexService);
            while(!shutdown)
            {
                logic();
                Utils::conditionVariableSleep(l, cvService, RELAYBOARD_LOGIC_INTERVAL);
            }
            // Disable/switch-off (for safety) and dispose each relay socket
            Relay *r;
            for(vector<Relay*>::iterator it = relayPins.begin(); it != relayPins.end(); it++)
            {
                r = *it;
                r->set(false);
                delete r;
            }
            relayPins.clear();
        }
        void RelayBoard::logic()
        {
            unique_lock<mutex> lock(lmutex);
            for(vector<Relay*>::iterator it = relayPins.begin(); it != relayPins.end(); it++)
                (*it)->logic();
        }
        Relay* RelayBoard::addRelay(int gpioPin)
        {
            unique_lock<mutex> lock(lmutex);
            Relay *r = new Relay(this, gpioPin);
            relayPins.push_back(r);
            return r;
        }
    }
}