

#include "Display.h"
#include "IC_74HC595.h"
#include "Sensors.h"
#include "Alarm.h"
#include "ServiceController.h"
#include "Configurator.h"

using BC::Services::Configurator;
using BC::Hardware::IC_74HC595;

namespace BC
{
    namespace Services
    {
        void Display::run()
        {
            // Load configuration
            {
                Configurator *c = static_cast<Configurator*>(controller->getServiceByName(SERVICETITLE_CONFIGURATOR));
                registerCount = c->get(CONFIG__DISPLAY__REGISTERS, 1, IC_74HC595_MAX_REGISTERS, DISPLAY_DEFAULT_REGISTERS);
                pinData = c->get(CONFIG__DISPLAY__PIN_DATA, GPIO_MIN, GPIO_MAX, DISPLAY_DEFAULT_PIN_DATA);
                pinLatch = c->get(CONFIG__DISPLAY__PIN_LATCH, GPIO_MIN, GPIO_MAX, DISPLAY_DEFAULT_PIN_LATCH);
                pinClock = c->get(CONFIG__DISPLAY__PIN_CLOCK, GPIO_MIN, GPIO_MAX, DISPLAY_DEFAULT_PIN_CLOCK);
                lightThreshold = c->get(CONFIG__DISPLAY__LIGHT_THRESHOLD, 1, 100, DISPLAY_DEFAULT_LIGHT_THRESHOLD);
                sequenceBuzz = (Sequence)c->get(CONFIG__DISPLAY__LIGHT_BUZZ, 0, NUMBER_OF_SEQUENCES - 1, DISPLAY_DEFAULT_SEQUENCE_LIGHT_BUZZ);
                sequenceHigh = (Sequence)c->get(CONFIG__DISPLAY__LIGHT_HIGH, 0, NUMBER_OF_SEQUENCES - 1, DISPLAY_DEFAULT_SEQUENCE_LIGHT_HIGH);
                sequenceLow = (Sequence)c->get(CONFIG__DISPLAY__LIGHT_LOW, 0, NUMBER_OF_SEQUENCES - 1, DISPLAY_DEFAULT_SEQUENCE_LIGHT_LOW);
            }
            // Set the shift-register pins to outputs
            controller->gpioSetPinMode(pinData, ServiceController::GPIO_Mode::Output);
            controller->gpioSetPinMode(pinLatch, ServiceController::GPIO_Mode::Output);
            controller->gpioSetPinMode(pinClock, ServiceController::GPIO_Mode::Output);
            // Get access to the alarm and sensor service's
            Alarm *serviceAlarm = static_cast<Alarm*>(controller->getServiceByName(SERVICETITLE_ALARM));
            Sensors *serviceSensors = static_cast<Sensors*>(controller->getServiceByName(SERVICETITLE_SENSORS));
            // Begin looping until the service is stopped
            long long time, delayTime = 0, delay;
            int lightMode = 0; // 0 = low, 1 = high, 2 = buzz
            unique_lock<mutex> cl(mutexService);
            while(!shutdown)
            {
                time = Utils::getEpochTimeMs();
                if(time > delayTime)
                {
                    unique_lock<mutex> lock(mutexThread);
                    int currLightMode;
                    if(serviceAlarm != 0 && serviceAlarm->isBuzzing())
                        currLightMode = 2;
                    else
                        currLightMode = serviceSensors != 0 && serviceSensors->getLight() >= lightThreshold;
                    // Check if the light-level has changed, if so reset the buffer
                    if(lightMode != currLightMode)
                    {
                        buffer = 0;
                        lightMode = currLightMode;
                    }
                    // Invoke the function responsible for manipulating the buffer based on the sequence
                    switch(lightMode == 1 ? sequenceHigh : lightMode == 2 ? sequenceBuzz : sequenceLow)
                    {
                        case Sequence::Failure:
                            delay = sequence_Failure();
                            break;
                        case Sequence::SingleLedTest:
                            delay = sequence_SingleLedTest();
                            break;
                        case Sequence::Time:
                            delay = sequence_Time();
                            break;
                        case Sequence::ManualBuffer:
                            delay = manualDelay;
                            break;
                        case Sequence::SymbolX:
                            delay = sequence_SymbolX();
                            break;
                        case Sequence::Offline:
                            delay = 100;
                            buffer = 0;
                            break;
                        case Sequence::AllLeds:
                            delay = sequence_AllLeds();
                            break;
                        default:
                            delay = 0;
                            break;
                    }
                    // Push the buffer to the shift-registers
                    if(controller->isHardwareEnabled())
                        IC_74HC595::write(controller, registerCount, pinData, pinClock, pinLatch, buffer);
                    // We don't sleep for the delay-time because we want
                    // the while-loop to be able to exit at any point
                    delayTime = time + delay;
                }
                Utils::conditionVariableSleep(cl, cvService, 50);
            }
            // Set lights to failure
            sequence_Failure();
            if(controller->isHardwareEnabled())
                IC_74HC595::write(controller, registerCount, pinData, pinClock, pinLatch, buffer);
        }
        void Display::changeBuffer(int value)
        {
            unique_lock<mutex> lock(mutexThread);
            buffer = value;
        }
        void Display::changeManualDelay(int delay)
        {
            unique_lock<mutex> lock(mutexThread);
            this->manualDelay = delay;
        }
        int Display::sequence_SingleLedTest()
        {
            if(buffer >= 16777216)
                buffer = 0;
            else
                buffer = (buffer << 1) | 0x1;
            return 350;
        }
        int Display::sequence_Time()
        {
            BC::Time time = Utils::getTime();
            int hours = time.hour, minutes = time.minute, seconds = time.second;
            // Build the buffer value to represent the time
            buffer = 0;
            if(hours >= 16)
            {
                buffer += DISPLAY_LED_H_16;
                hours -= 16;
            }
            if(hours >= 8)
            {
                buffer += DISPLAY_LED_H_8;
                hours -= 8;
            }
            if(hours >= 4)
            {
                buffer += DISPLAY_LED_H_4;
                hours -= 4;
            }
            if(hours >= 2)
            {
                buffer += DISPLAY_LED_H_2;
                hours -= 2;
            }
            if(hours == 1)
                buffer += DISPLAY_LED_H_1;
            
            if(minutes >= 32)
            {
                buffer += DISPLAY_LED_M_32;
                minutes -= 32;
            }
            if(minutes >= 16)
            {
                buffer += DISPLAY_LED_M_16;
                minutes -= 16;
            }
            if(minutes >= 8)
            {
                buffer += DISPLAY_LED_M_8;
                minutes -= 8;
            }
            if(minutes >= 4)
            {
                buffer += DISPLAY_LED_M_4;
                minutes -= 4;
            }
            if(minutes >= 2)
            {
                buffer += DISPLAY_LED_M_2;
                minutes -= 2;
            }
            if(minutes == 1)
                buffer += DISPLAY_LED_M_1;

            if(seconds >= 32)
            {
                buffer += DISPLAY_LED_S_32;
                seconds -= 32;
            }
            if(seconds >= 16)
            {
                buffer += DISPLAY_LED_S_16;
                seconds -= 16;
            }
            if(seconds >= 8)
            {
                buffer += DISPLAY_LED_S_8;
                seconds -= 8;
            }
            if(seconds >= 4)
            {
                buffer += DISPLAY_LED_S_4;
                seconds -= 4;
            }
            if(seconds >= 2)
            {
                buffer += DISPLAY_LED_S_2;
                seconds -= 2;
            }
            if(seconds == 1)
                buffer += DISPLAY_LED_S_1;
            return 1000;
        }
        int Display::sequence_SymbolX()
        {
            buffer = DISPLAY_LED_H_8 + DISPLAY_LED_H_1 + DISPLAY_LED_M_4 + DISPLAY_LED_M_2 + DISPLAY_LED_S_8 + DISPLAY_LED_S_1;
            return 100;
        }
        int Display::sequence_Failure()
        {
            // Toggle error LED's on and off
            if(buffer != 0)
                buffer = 0;
            else
                buffer = DISPLAY_LED_ERROR_1 + DISPLAY_LED_ERROR_2 + DISPLAY_LED_ERROR_3;
            return 50;
        }
        int Display::sequence_AllLeds()
        {
            buffer = DISPLAY_LED_H_1 + DISPLAY_LED_H_2 + DISPLAY_LED_H_4 + DISPLAY_LED_H_8 + DISPLAY_LED_H_16 +
                    DISPLAY_LED_M_1 + DISPLAY_LED_M_2 + DISPLAY_LED_M_4 + DISPLAY_LED_M_8 + DISPLAY_LED_M_16 + DISPLAY_LED_M_32 +
                    DISPLAY_LED_S_1 + DISPLAY_LED_S_2 + DISPLAY_LED_S_4 + DISPLAY_LED_S_8 + DISPLAY_LED_S_16 + DISPLAY_LED_S_32;
            return 50;
        }
    }
}