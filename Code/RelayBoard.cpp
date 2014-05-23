
#include "RelayBoard.h"

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
            conditionFuncs["alarm_buzzing"] = RelayConditionFunctions::alarmBuzzing;
        }
        void RelayBoard::run()
        {
            // Add virtual sockets
            {
                unique_lock<mutex> lock(lmutex);
                Configurator *c = static_cast<Configurator*>(controller->getServiceByName(SERVICETITLE_CONFIGURATOR));
                int totalSocks = c->get(CONFIG__RELAYBOARD_TOTAL_SOCKETS, RELAYBOARD_DEFAULT_SOCKETS);
                for(int i = 0; i < totalSocks; i++)
                    relayPins.push_back(new Relay(this, i));
                cout << getTitle() << ": added " << totalSocks << " relay sockets!" << endl;
            }
            // Periodically execute logic until shutdown
            unique_lock<mutex> lSleep(mutexService);
            while(!shutdown)
            {
                logic();
                Utils::conditionVariableSleep(lSleep, cvService, RELAYBOARD_LOGIC_INTERVAL);
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
    }
}
