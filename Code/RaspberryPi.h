
#ifndef RASPBERRYPI_H
#define	RASPBERRYPI_H

#include <iostream>
using std::cerr;
using std::endl;

#include <fcntl.h>
#include <sys/mman.h>

#include <mutex>
using std::mutex;
using std::unique_lock;

#include "Utils.h"
using BC::Utils;

#define RASPBERRYPI_ADDRESS_BCM2708             0x20000000
#define RASPBERRYPI_ADDRESS_GPIO_PINS           (RASPBERRYPI_ADDRESS_BCM2708 + 0x200000)
#define RASPBERRYPI_SIZE_BLOCK                  4*1024
#define RASPBERRYPI_SETMODE_INPUT(pin)          *(memoryGPIOSafe+((pin)/10)) &= ~(7<<(((pin)%10)*3))
#define RASPBERRYPI_SETMODE_OUTPUT(pin)         *(memoryGPIOSafe+((pin)/10)) |=  (1<<(((pin)%10)*3))

namespace BC
{
    namespace Hardware
    {
        class RaspberryPi
        {
        public:
            enum PinMode
            {
                Input,
                Output
            };
        private:
            mutex               mutexPi;
            bool                isSetup;
            volatile unsigned   *memoryGPIOSafe;
        public:
            bool setup()
            {
                unique_lock<mutex> l(mutexPi);
                if(isSetup)
                {
                    cerr << "Raspberry Pi: called setup more than once!" << endl;
                    return false;
                }
                int fdMem;
                // Open access to the kernel memory
                // -- http://linux.die.net/man/4/mem
                // -- http://pubs.opengroup.org/onlinepubs/7908799/xsh/open.html
                // -- -- O_RDWR = read-write flag
                // -- -- O_SYNC = ensures, on an OS level, all data has been written before reading.
                if((fdMem = open("/dev/mem", O_RDWR | O_SYNC)) < 0)
                {
                    cerr << "Raspberry Pi: failed to access kernel memory!" << endl;
                    return false;
                }
                // Create a pointer/memory-map to the raw kernel memory
                void *memoryGPIO = mmap(
                        0,                                      // Start address.
                        RASPBERRYPI_SIZE_BLOCK,                 // Length of memoryGPIO
                        PROT_READ | PROT_WRITE,                 // Pages can be read/written-to.
                        MAP_SHARED,                             // Changes to memory are made public all processes.
                        fdMem,                                  // File-descriptor to the kernel memory
                        RASPBERRYPI_ADDRESS_GPIO_PINS           // Offset address
                        );
                // Dispose file-descriptor, we can now use the memory-map
                close(fdMem);
                // Check our memory-map was created
                if(memoryGPIO == MAP_FAILED)
                {
                    cerr << "Raspberry Pi: failed to create memory-map to kernel memory!" << endl;
                    return false;
                }
                // Setup a safe (volatile) pointer to the memory
                memoryGPIOSafe = (volatile unsigned *)memoryGPIO;
                isSetup = true;
                return true;
            }
            void gpioMode(int pin, RaspberryPi::PinMode mode)
            {
                switch(mode)
                {
                    case RaspberryPi::PinMode::Input:
                        RASPBERRYPI_SETMODE_INPUT(pin);
                        break;
                    case RaspberryPi::PinMode::Output:
                        RASPBERRYPI_SETMODE_INPUT(pin);
                        RASPBERRYPI_SETMODE_OUTPUT(pin);
                        break;
                }
            }
            void gpioWrite(int pin, bool value)
            {
                unique_lock<mutex> l(mutexPi);
                /*
                 * HIGH offset of 7 for the first 32 pins, 8 for the rest.
                 * LOW offset of 10 for the first 32 pins, 11 for the rest.
                 * 
                 * Simply shifts 1 by the pin-number to turn on/off the pin.
                 */
                if(value)
                    *(memoryGPIOSafe+7) = 1 << pin;
                else
                    *(memoryGPIOSafe+10) = 1 << pin;
            }
            bool gpioRead(int pin)
            {
                unique_lock<mutex> l(mutexPi);
                /*
                 * Offset of 13 for the first 28 pins, 14 for the rest.
                 * 
                 * Simply reads the memory for pin on/off values and filters
                 * by the pin number.
                 */
                return (
                                *(memoryGPIOSafe+13)
                                &
                                (1 << pin) 
                        ) != 0;
            }
        };
    }
}

#endif	/* RASPBERRYPI_H */
