
#ifndef TIME_H
#define	TIME_H

#include <string>
using std::string;

#include "Utils.h"
namespace BC
{
    class Utils;
}
using BC::Utils;

namespace BC
{
    struct Time
    {
        int hour, minute, second;
        Time() : hour(0), minute(0), second(0)
        {}
        Time(int hour, int minute, int second) : hour(hour), minute(minute), second(second)
        {}
        // Expected format: hh:mm:ss.
        static bool parse(string text, Time &dest);
        inline bool operator==(const Time &at) const
        {
            return hour == at.hour && minute == at.minute && second == at.second;
        }
        inline bool operator!=(const Time &at) const
        {
            return !(*this == at);
        }
        inline bool operator>(const Time &at) const
        {
            if(hour > at.hour)
                return true;
            else if(hour == at.hour)
            {
                if(minute > at.minute)
                    return true;
                else if(minute == at.minute)
                    return second > at.second;
            }
            return false;
        }
        inline bool operator>=(const Time &at) const
        {
            return *this == at || *this > at;
        }
        inline bool operator<(const Time &at) const
        {
            return *this != at && !(*this > at);
        }
        inline bool operator<=(const Time &at) const
        {
            return *this == at || !(*this > at);
        }
    };
}

#endif	/* TIME_H */

