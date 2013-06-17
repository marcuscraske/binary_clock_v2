#ifndef ALARM_H
#define	ALARM_H

#include <algorithm>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>

#include "Definitions.h"
#include "IService.h"
#include "ServiceController.h"

#include "DateTime.h"
using BC::DateTime;

#include "Utils.h"
using BC::Utils;

using std::vector;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::stringstream;

#define ALARM_DEFAULT_PIN_BUZZER        7
#define ALARM_DEFAULT_BUZZES            20
#define ALARM_DEFAULT_BUZZ_LENGTH       1

namespace BC
{
    namespace Services
    {
        class Alarm : public IService
        {
        private:
            // Fields --------------------------------------------------------->
            // --Alarm-times are stored in chronological order; hence only the
            // -- first struct needs checking every cycle.
            vector<DateTime> alarms;
            // -- Number of alarm buzzes to make when sounding the alarm.
            int alarmBuzzes,
            // -- The length of each alarm buzz when sounding the alarm.
                alarmBuzzLength,
            // -- The GPIO pin with the buzzer.
                alarmBuzzerPin;
            // -- The buzzing flag, to indicate if the alarm is currently
            // -- buzzing.
            bool buzzing;
        public:
            // Constructors --------------------------------------------------->
            Alarm(ServiceController * const controller) : IService(controller), buzzing(false), alarmBuzzes(ALARM_DEFAULT_BUZZES), alarmBuzzLength(ALARM_DEFAULT_BUZZ_LENGTH), alarmBuzzerPin(ALARM_DEFAULT_PIN_BUZZER) {}
            // Member Functions ----------------------------------------------->
            void run();
            void sort();
        public:
            // -- Saves all alarm times to file via the configurator service.
            // -- Not thread-safe.
            void save();
            // -- Adds a new alarm-time for sounding the alarm.
            void add(DateTime time)
            {
                alarms.push_back(time);
                sort();
                cout << "Added an alarm for " << time.day << "/" << time.month << "/" << time.year << " " << time.hour << ":" << time.minute << ":" << time.second << "." << endl;
                save();
            }
            // -- Removes an alarm time, if it can be found.
            void remove(DateTime time);
            // -- Clears all alarms.
            inline void reset()
            {
                alarms.clear();
            }
            // -- Returns the number of alarm times. This is not thread-safe,
            // -- therefore you should lock this service's mutex!
            inline int count() { return alarms.size(); }
            // -- Retrieves an alarm; this is not thread-safe, you should lock
            // -- this service's mutex!
            inline DateTime getAlarm(int index) { return alarms[index]; }
            // -- Indicates if the alarm is currently buzzing.
            inline bool isBuzzing() { return buzzing; }
            // -- Indicates the number of buzzes made for an alarm sounding.
            inline int getAlarmBuzzes() { return alarmBuzzes; }
            // -- Indicates the length of each buzz cycle during an alarm
            // -- sounding.
            inline int getAlarmBuzzLength() { return alarmBuzzLength; }
            string getTitle() { return SERVICETITLE_ALARM; }
        };
        inline ostream& operator<<(ostream &out, DateTime const &at)
        {
            out << at.year << " " << at.month << " " << at.day << " " << at.hour << " " << at.minute << " " << at.second;
            return out;
        }
        inline istream& operator>>(istream &in, DateTime &at)
        {
            if(
            !(in >> at.year) ||
            !(in >> at.month) ||
            !(in >> at.day) ||
            !(in >> at.hour) ||
            !(in >> at.minute) ||
            !(in >> at.second) ||
                    at.year < 2013 ||
                    at.year > 9999 ||
                    at.month < 1 ||
                    at.month > 12 ||
                    at.day < 1 ||
                    at.day > 31 ||
                    at.hour < 1 ||
                    at.hour > 23 ||
                    at.minute < 1 ||
                    at.minute > 59 ||
                    at.second < 1 ||
                    at.second > 59
                    )
            {
                in.setstate(ios::badbit);
            }
            return in;
        }
    }
}

#endif	/* ALARM_H */

