#ifndef IC_74HC595_H
#define	IC_74HC595_H

#include "ServiceController.h"
using BC::ServiceController;

namespace BC
{
    namespace Hardware
    {
        class IC_74HC595
        {
        public:
            inline static void write(ServiceController *controller, uint8_t numberOfShiftRegisters, uint8_t pinData, uint8_t pinClock, uint8_t pinLatch, int value)
            {
                // Place latch to low, to write data
                controller->gpioWrite(pinLatch, false);
                // Write eight bits for each shift register
                for(int i = (8 * numberOfShiftRegisters) - 1; i >= 0; --i)
                {
                    // Set data-pin to on or off
                    controller->gpioWrite(pinData, value & (1 << i));
                    // Inform the IC to read the data-pin
                    controller->gpioWrite(pinClock, true);
                    controller->gpioWrite(pinClock, false);
                }
                // Put the latch back to high
                controller->gpioWrite(pinLatch, true);
            }
        };
    }
}

#endif	/* IC_74HC595_H */