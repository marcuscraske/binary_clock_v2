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
 *      File:           IC_MCP3008.h
 *      Path:           BC/Hardware/IC_MCP3008.h
 * 
 *      Change-Log:
 *                      2013-06-25      Added header.
 * 
 * *****************************************************************************
 * Used to interact with the hardware integrated circuit "MCP3008", an
 * 8-bit analogue to digital converter (ADC).
 * *****************************************************************************
 */
#ifndef MCP3008_H
#define	MCP3008_H

#include <stdint.h>

#include "ServiceController.h"
using BC::ServiceController;

namespace BC
{
    namespace Hardware
    {
        class IC_MCP3008
        {
        public:
            enum MCP3008_PINS
            {
                Pin0 = 0,
                Pin1 = 1,
                Pin2 = 2,
                Pin3 = 3,
                Pin4 = 4,
                Pin5 = 5,
                Pin6 = 6,
                Pin7 = 7
            };
            static int read(ServiceController *controller, MCP3008_PINS pin, uint8_t pinDataIn, uint8_t pinDataOut, uint8_t pinClock, uint8_t pinChipSelect);
        };
    }
}

#endif

