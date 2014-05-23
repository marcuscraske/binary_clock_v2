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
 *      File:           Sensors.h
 *      Path:           BC/Services/Sensors.h
 * 
 *      Change-Log:
 *                      2013-06-25      Added header.
 * 
 * *****************************************************************************
 * The sensor service, used to periodically poll sensor data.
 * *****************************************************************************
 */
#ifndef SENSORS_H
#define	SENSORS_H

#include "Definitions.h"

#include <unistd.h>

#include "IService.h"
using BC::Services::IService;

#include "IC_MCP3008.h"
using BC::Hardware::IC_MCP3008;

#include "PhotoResistor.h"
using BC::Hardware::PhotoResistor;

#include "TMP36.h"
using BC::Hardware::TMP36;

#include "Utils.h"
using BC::Utils;

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
