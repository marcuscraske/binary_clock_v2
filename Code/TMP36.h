
#ifndef TMP36_H
#define	TMP36_H

#include "IC_MCP3008.h"
using BC::Hardware::IC_MCP3008;

#include "ServiceController.h"
using BC::ServiceController;

namespace BC
{
    namespace Hardware
    {
        class TMP36
        {
            inline static int getRaw(ServiceController *controller, IC_MCP3008::MCP3008_PINS pin, uint8_t pinDataIn, uint8_t pinDataOut, uint8_t pinClock, uint8_t pinChipSelect)
            {
                return IC_MCP3008::read(controller, pin, pinDataIn, pinDataOut, pinClock, pinChipSelect);
            }
        public:
            inline static double getTemperature_Farenheit(ServiceController *controller, double volts, IC_MCP3008::MCP3008_PINS pin, uint8_t pinDataIn, uint8_t pinDataOut, uint8_t pinClock, uint8_t pinChipSelect)
            {
                return convertToFarenheit(getTemperature_Celcius(controller, volts, pin, pinDataIn, pinDataOut, pinClock, pinChipSelect)); //(f - 32.0) / 1.8 = c
            }
            inline static double getTemperature_Celcius(ServiceController *controller, double volts, IC_MCP3008::MCP3008_PINS pin, uint8_t pinDataIn, uint8_t pinDataOut, uint8_t pinClock, uint8_t pinChipSelect)
            {
                return ((getRaw(controller, pin, pinDataIn, pinDataOut, pinClock, pinChipSelect) * ((volts * 1000) / 1024)) - 500) / 10; // ([voltage in millivolts] - 500) / 10; volts to millivolts = [value] * ([[volts e.g. 3.3 or 5]v * 1000] / 1024)
            }
            inline static double convertToFarenheit(double celcius)
            {
                return (celcius * 1.8) + 32.; //(f + 32.0) / 1.8
            }
        };
    }
}
#endif	/* TMP36_H */

