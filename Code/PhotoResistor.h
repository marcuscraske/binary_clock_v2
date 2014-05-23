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
 *      File:           PhotoResistor.h
 *      Path:           BC/Hardware/PhotoResistor.h
 * 
 *      Change-Log:
 *                      2013-06-25      Added header.
 * 
 * *****************************************************************************
 * Used to interact with a photo-resistor hardware component, which scales a
 * voltage between 0.0 to <max analogue voltage> based on the amount of
 * light.
 * *****************************************************************************
 */
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
            static int getRaw(ServiceController *controller, IC_MCP3008::MCP3008_PINS pin, uint8_t pinDataIn, uint8_t pinDataOut, uint8_t pinClock, uint8_t pinChipSelect)
            {
                return IC_MCP3008::read(controller, pin, pinDataIn, pinDataOut, pinClock, pinChipSelect);
            }
            static double getPercent(ServiceController *controller, IC_MCP3008::MCP3008_PINS pin, uint8_t pinDataIn, uint8_t pinDataOut, uint8_t pinClock, uint8_t pinChipSelect)
            {
                return (getRaw(controller, pin, pinDataIn, pinDataOut, pinClock, pinChipSelect) / 1023.0) * 100.0; // divide by zero protection
            }
        };
    }
}

#endif	/* PHOTORESISTOR_H */
