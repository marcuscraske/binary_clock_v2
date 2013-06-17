#ifndef PHOTORESISTOR_H
#define	PHOTORESISTOR_H

#include "IC_MCP3008.h"
using BC::Hardware::IC_MCP3008;

#include "ServiceController.h"
using BC::ServiceController;

namespace BC
{
    namespace Hardware
    {
        class PhotoResistor
        {
        public:
            inline static int getRaw(ServiceController *controller, IC_MCP3008::MCP3008_PINS pin, uint8_t pinDataIn, uint8_t pinDataOut, uint8_t pinClock, uint8_t pinChipSelect)
            {
                return IC_MCP3008::read(controller, pin, pinDataIn, pinDataOut, pinClock, pinChipSelect);
            }
            inline static double getPercent(ServiceController *controller, IC_MCP3008::MCP3008_PINS pin, uint8_t pinDataIn, uint8_t pinDataOut, uint8_t pinClock, uint8_t pinChipSelect)
            {
                return (getRaw(controller, pin, pinDataIn, pinDataOut, pinClock, pinChipSelect) / 1023.0) * 100.0; // divide by zero protection
            }
        };
    }
}

#endif	/* PHOTORESISTOR_H */
