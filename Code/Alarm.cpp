
#include "Alarm.h"
#include "Configurator.h"

using BC::Services::Configurator;

namespace BC
{
    namespace Services
    {
        void Alarm::save()
        {
            // Write each alarm to a new-line, we'll then save the string
            stringstream ss;
            for(vector<DateTime>::iterator it = alarms.begin(); it != alarms.end(); it++)
                ss << (*it) << '\n';
            Configurator *c = static_cast<Configurator*>(controller->getServiceByName(SERVICETITLE_CONFIGURATOR));
            c->set(CONFIG__ALARM__TIMES, ss.str());
        }
        void Alarm::run()
        {
            // Load configuration
            {
                Configurator *c = static_cast<Configurator*>(controller->getServiceByName(SERVICETITLE_CONFIGURATOR));
                alarmBuzzes = c->get(CONFIG__ALARM__BUZZES, ALARM_DEFAULT_BUZZES);
                alarmBuzzLength = c->get(CONFIG__ALARM__BUZZLENGTH, ALARM_DEFAULT_BUZZ_LENGTH);
                alarmBuzzerPin = c->get(CONFIG__ALARM__BUZZER_PIN, ALARM_DEFAULT_PIN_BUZZER);
                string alarmData = c->get(CONFIG__ALARM__TIMES, std::string());
                if(alarmData.length() > 0)
                {
                    // Parse each alarm - each line is an alarm
                    stringstream ss;
                    ss << alarmData;
                    string line;
                    while(getline(ss, line, '\n'))
                    {
                        stringstream ssL;
                        ssL << line;
                        DateTime at;
                        if(ssL >> at)
                            alarms.push_back(at);
                        else
                            cerr << getTitle() << ": failed to add alarm '" << alarmData << "'!" << endl;
                    }
                }
            }
            // Set the buzzer-pin as an output
            controller->gpioSetPinMode(alarmBuzzerPin, ServiceController::GPIO_Mode::Output);
            // Loop until the service is stopped, checking for an alarm to be raised
            bool surpassed;
            int surpassedCycle = -1; // Allows buzzing across multiple cycles, allowing the service to shutdown quickly if required.
            long long lastBuzz;
            unique_lock<mutex> sl(mutexService);
            while(!shutdown)
            {
                // Check if the next cycle of the alarm is due...
                if(surpassedCycle >= 0 && Utils::getEpochTimeMs() - lastBuzz >= (long long)alarmBuzzLength)
                {
                    buzzing = surpassedCycle % 2 == 0;
                    controller->gpioWrite(alarmBuzzerPin, buzzing);
                    if(++surpassedCycle >= alarmBuzzes * 2)
                    {
                        // No more buzzes...end of alarm cycle; we can now update the configuration file
                        // with the current collection of times.
                        surpassedCycle = -1;
                        unique_lock<mutex> lock(mutexThread);
                        save();
                    }
                    else
                        lastBuzz = Utils::getEpochTimeMs();
                }
                surpassed = false;
                // Lock the collection
                {
                    unique_lock<mutex> lock(mutexThread);
                    if(alarms.size() > 0)
                    {
                        // Check the first alarm and see if it has been surpassed
                        DateTime timeNow = Utils::getDateTime();
                        DateTime item = alarms.at(0);
                        surpassed = timeNow >= item;
                    }
                    if(surpassed)
                    {
                        cout << getTitle() << ": alarm surpassed: " << *alarms.begin() << endl;
                        // Remove the item
                        alarms.erase(alarms.begin());
                    }
                }
                if(surpassed)
                {
                    surpassedCycle = 0;
                    lastBuzz = -1;
                }
                // Sleep to avoid CPU-hogging. We could sleep up until the time, however
                // the time of the Pi could change; additionally this is a mission-critical
                // service, therefore we don't mind constantly checking.
                Utils::conditionVariableSleep(sl, cvService, 1000);
            }
            // Ensure buzzer is off
            controller->gpioWrite(alarmBuzzerPin, false);
            buzzing = false;
        }
        void Alarm::remove(DateTime time)
        {
            unique_lock<mutex> lock(mutexThread);
            vector<DateTime>::iterator it = alarms.begin();
            while(*it != time)
                it++;
            if(*it == time)
            {
                alarms.erase(it);
                save();
            }
        }
        void Alarm::sort()
        {
            unique_lock<mutex> lock(mutexThread);
            std::sort(alarms.begin(), alarms.end(), DateTime::comparator);
        }
    }
}