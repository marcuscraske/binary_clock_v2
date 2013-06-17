#ifndef SENSORS_H
#define	SENSORS_H

#include <unistd.h>

#include "Definitions.h"
#include "IService.h"
#include "IC_MCP3008.h"
#include "PhotoResistor.h"
#include "TMP36.h"
#include "Utils.h"

using BC::Utils;

#define SENSORS_DEFAULT_MCP_TMP36                IC_MCP3008::Pin1
#define SENSORS_DEFAULT_MCP_PHOTORESISTOR        IC_MCP3008::Pin0
#define SENSORS_DEFAULT_PIN_CLOCK                15
#define SENSORS_DEFAULT_PIN_DATA_OUT             16
#define SENSORS_DEFAULT_PIN_DATA_IN              1
#define SENSORS_DEFAULT_PIN_CHIP_SELECT          4
#define SENSORS_DEFAULT_POLL_RATE_MS             500
#define SENSORS_VOLTAGE                         3.3
#define SENSORS_MCP_TMP36_ERROR                 4
#define SENSORS_MCP_PHOTORESISTOR_ERROR         7

namespace BC
{
    namespace Services
    {
        class Sensors : public IService
        {
        private:
            // Fields --------------------------------------------------------->
            // -- Recorded temperature and light values for the current and last
            // -- cycles.
            double      temperature,
                        temperatureLast,
                        light,
                        lightLast;
            // -- Pins for the MCP ADC (analogue to digital converter)
            int         pinDataIn,
                        pinDataOut,
                        pinClock,
                        pinChipSelect,
            // -- Pins for the sensors connected to the MCP (pins relative to
            // -- MCP).
                        pinMcpTMP36,
                        pinMcpPhotoResistor,
            // -- Poll-rate for updating sensor data.
                        pollrate;
        public:
            // Constructors --------------------------------------------------->
            Sensors(ServiceController * const controller) : IService(controller), pinDataIn(SENSORS_DEFAULT_PIN_DATA_IN), pinDataOut(SENSORS_DEFAULT_PIN_DATA_OUT), pinClock(SENSORS_DEFAULT_PIN_CLOCK), pinChipSelect(SENSORS_DEFAULT_PIN_CHIP_SELECT), pinMcpTMP36(SENSORS_DEFAULT_MCP_TMP36), pinMcpPhotoResistor(SENSORS_DEFAULT_MCP_PHOTORESISTOR), pollrate(SENSORS_DEFAULT_POLL_RATE_MS), temperature(0), light(0), temperatureLast(0), lightLast(0) {}
            // Member Functions ----------------------------------------------->
            void run();
            // Member Functions - Accessors ----------------------------------->
            inline double getLight() { return light; }
            inline double getTemperature() { return temperature; }
            string getTitle() { return SERVICETITLE_SENSORS; }
        };
    }
}

#endif	/* SENSORS_H */

