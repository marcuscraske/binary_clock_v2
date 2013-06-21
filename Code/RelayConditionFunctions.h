
#ifndef RELAYCONDITIONFUNCTIONS_H
#define	RELAYCONDITIONFUNCTIONS_H

#include <string>
using std::string;

#include "ServiceController.h"
using BC::ServiceController;

#include "Time.h"
using BC::Time;

#include "Alarm.h"
#include "Sensors.h"
namespace BC
{
    namespace Services
    {
        class Sensors;
        class Alarm;
    }
}
using BC::Services::Alarm;
using BC::Services::Sensors;

namespace BC
{
    namespace Services
    {
        class RelayConditionFunctions
        {
        public:
            // Returns true if the temperature sensor is between param1 (int) and param2 (int).
            static bool temperature(ServiceController *controller, string param1, string param2);
            // Returns true if the light sensor is between param1 (int) and param2 (int).
            static bool light(ServiceController *controller, string param1, string param2);
            // Returns true if the current time is between the two times (hh:mm:ss format).
            static bool timeOfDay(ServiceController *controller, string param1, string param2);
            // Returns true if the buzzer of the alarm service is buzzing; no parameters.
            static bool alarmBuzzing(ServiceController *controller, string param1, string param2);
        };
    }
}

#endif	/* RELAYCONDITIONFUNCTIONS_H */

