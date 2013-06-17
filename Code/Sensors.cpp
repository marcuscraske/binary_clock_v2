
#include "Sensors.h"
#include "Configurator.h"

using BC::Services::Configurator;
using BC::Hardware::PhotoResistor;
using BC::Hardware::TMP36;

namespace BC
{
    namespace Services
    {
        void Sensors::run()
        {
            // Load configuration
            {
                Configurator *c = static_cast<Configurator*>(controller->getServiceByName(SERVICETITLE_CONFIGURATOR));
                // -- Pins of MCP
                pinDataIn = c->get(CONFIG__SENSORS__PIN_DATA_IN, GPIO_MIN, GPIO_MAX, SENSORS_DEFAULT_PIN_DATA_IN);
                pinDataOut = c->get(CONFIG__SENSORS__PIN_DATA_OUT, GPIO_MIN, GPIO_MAX, SENSORS_DEFAULT_PIN_DATA_OUT);
                pinClock = c->get(CONFIG__SENSORS__PIN_CLOCK, GPIO_MIN, GPIO_MAX, SENSORS_DEFAULT_PIN_CLOCK);
                pinChipSelect = c->get(CONFIG__SENSORS__PIN_CHIP_SELECT, GPIO_MIN, GPIO_MAX, SENSORS_DEFAULT_PIN_CHIP_SELECT);
                // -- MCP Sensor Pins
                pinMcpTMP36 = (IC_MCP3008::MCP3008_PINS)c->get(CONFIG__SENSORS__MCP_PIN_TMP36, MCP3008_PIN_MIN, MCP3008_PIN_MAX, SENSORS_DEFAULT_MCP_TMP36);
                pinMcpPhotoResistor = (IC_MCP3008::MCP3008_PINS)c->get(CONFIG__SENSORS__MCP_PIN_PHOTORESISTOR, MCP3008_PIN_MIN, MCP3008_PIN_MAX, SENSORS_DEFAULT_MCP_PHOTORESISTOR);
                // -- Misc
                pollrate = c->get(CONFIG__SENSORS__POLLRATE, 1, 99999, SENSORS_DEFAULT_POLL_RATE_MS);
            }
            // Setup the output/input mode(s) of pins
            controller->gpioSetPinMode(pinDataIn, ServiceController::GPIO_Mode::Output);
            controller->gpioSetPinMode(pinDataOut, ServiceController::GPIO_Mode::Input);
            controller->gpioSetPinMode(pinClock, ServiceController::GPIO_Mode::Output);
            controller->gpioSetPinMode(pinChipSelect, ServiceController::GPIO_Mode::Output);
            // Poll data and sleep for the poll-rate, until the service is stopped
            double lightT, temperatureT;
            unique_lock<mutex> cl(mutexService);
            while(!shutdown)
            {
                // Fetch values
                lightT = controller->isHardwareEnabled() ? PhotoResistor::getPercent(controller, (IC_MCP3008::MCP3008_PINS)pinMcpPhotoResistor, pinDataIn, pinDataOut, pinClock, pinChipSelect) : 0.0;
                temperatureT = controller->isHardwareEnabled() ? TMP36::getTemperature_Celcius(controller, SENSORS_VOLTAGE, (IC_MCP3008::MCP3008_PINS)pinMcpTMP36, pinDataIn, pinDataOut, pinClock, pinChipSelect) : 0.0;
                // Check against error margins (in-case of sudden fluctuation)
                if(Utils::abs(lightT - lightLast) <= SENSORS_MCP_PHOTORESISTOR_ERROR)
                    light = lightT;
                lightLast = lightT;
                if(Utils::abs(temperatureT - temperatureT) <= SENSORS_MCP_TMP36_ERROR)
                    temperature = temperatureT;
                temperatureLast = temperatureT;
                Utils::conditionVariableSleep(cl, cvService, pollrate);
            }
        }
    }
}
