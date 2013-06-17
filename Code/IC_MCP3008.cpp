#include "IC_MCP3008.h"

namespace BC
{
    namespace Hardware
    {
        int IC_MCP3008::read(ServiceController *controller, MCP3008_PINS pin, uint8_t pinDataIn, uint8_t pinDataOut, uint8_t pinClock, uint8_t pinChipSelect)
        {
            if(pin < 0 || pin > 7)
                return -1;
            // With help from: http://pastebin.com/62rXCzej
            // Toggle chip-select for the MCP3008 to accept clock low-state
            controller->gpioWrite(pinChipSelect, true);
            controller->gpioWrite(pinClock, false);
            controller->gpioWrite(pinChipSelect, false);

            // Write 5-bits of data to the MCP3008
            // Structure:
            //          <11 to state start-bit and single-ended input mode>
            //          <three bits 0-7 to state the pin being read>
            // Source: http://ww1.microchip.com/downloads/en/DeviceDoc/21295d.pdf, page 19
            int value = pin;        // Pin (0-7) being read
            value |= 0x18;          // pin + 11000 (24 / 0x18)
            value <<= 3;            // Shift three places 11<pin>000
            for(int i = 0; i < 5; i++)
            {
                // 10000000 (0x80 / 128)
                controller->gpioWrite(pinDataIn, value & 0x80 ? true : false);
                value <<= 1;
                // Toggle clock to cause the MCP3008 to read the value of the data-in pin
                controller->gpioWrite(pinClock, true);
                controller->gpioWrite(pinClock, false);
            }

            // Read 12-bits form the MCP3008 (null bit, empty bit and 10 bits representing the value of the pin being read)
            value = 0;
            for(int i = 0; i < 12; i++)
            {
                // Toggle clock to cause the MCP3008 to write the next bit of data to data-out pin
                controller->gpioWrite(pinClock, true);
                controller->gpioWrite(pinClock, false);
                // Shift the current value by one, ready for the next bit of data to be read
                value <<= 1;
                // Read the next bit of data
                if(controller->gpioRead(pinDataOut))
                    value |= 0x1;
            }
            // Toggle chip-select pin, end of read
            controller->gpioWrite(pinChipSelect, true);
            // Get rid of the first bit (null)
            value /= 2;
        }
    }
}
