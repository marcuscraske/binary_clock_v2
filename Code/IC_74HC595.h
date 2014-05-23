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
 *      File:           IC_74HC595.h
 *      Path:           BC/Hardware/74HC595.h
 * 
 *      Change-Log:
 *                      2013-06-25      Added header.
 * 
 * *****************************************************************************
 * Used to interact with the hardware integrated-circuit "74HC595", an 8-bit
 * shift-register.
 * *****************************************************************************
 */
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
            static void write(ServiceController *controller, uint8_t numberOfShiftRegisters, uint8_t pinData, uint8_t pinClock, uint8_t pinLatch, int value)
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