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

