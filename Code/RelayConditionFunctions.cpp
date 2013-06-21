
#include "RelayConditionFunctions.h"

namespace BC
{
    namespace Services
    {
        bool RelayConditionFunctions::temperature(ServiceController *controller, string param1, string param2)
        {
            int p1, p2;
            if(Utils::tryParse(param1, p1) && Utils::tryParse(param2, p2))
            {
                Sensors *s = static_cast<Sensors*>(controller->getServiceByName(SERVICETITLE_SENSORS));
                return s != 0 && s->getTemperature() >= p1 && s->getTemperature() <= p2;
            }
            return false;
        }
        bool RelayConditionFunctions::light(ServiceController *controller, string param1, string param2)
        {
            int p1, p2;
            if(Utils::tryParse(param1, p1) && Utils::tryParse(param2, p2))
            {
                Sensors *s = static_cast<Sensors*>(controller->getServiceByName(SERVICETITLE_SENSORS));
                return s != 0 && s->getLight() >= p1 && s->getLight() <= p2;
            }
            return false;
        }
        bool RelayConditionFunctions::timeOfDay(ServiceController *controller, string param1, string param2)
        {
            Time p1, p2;
            if(Time::parse(param1, p1) && Time::parse(param2, p2))
            {
                Time tn = Utils::getTime();
                return tn >= p1 && tn <= p2;
            }
            else
                return false;
        }
        bool RelayConditionFunctions::alarmBuzzing(ServiceController *controller, string param1, string param2)
        {
            Alarm *a = static_cast<Alarm*>(controller->getServiceByName(SERVICETITLE_ALARM));
            return a != 0 && a->isBuzzing();
        }
    }
}