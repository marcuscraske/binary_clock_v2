
#ifndef DATETIME_H
#define	DATETIME_H

// Note: format of string is: yyyy mm dd HH MM ss

#include <iostream>
#include <ostream>
#include <istream>

using std::ostream;
using std::istream;
using std::ios;

namespace BC
{
    struct DateTime
    {
    public:
        int year, month, day, hour, minute, second;
        DateTime() {}
        DateTime(int year, int month, int day, int hour, int minute, int second) : year(year), month(month), day(day), hour(hour), minute(minute), second(second) {}
        inline bool operator==(const DateTime &at) const { return year == at.year && month == at.month && day == at.day && hour == at.hour && minute == at.minute && second == at.second; }
        inline bool operator!=(const DateTime &at) const { return !(*this == at); }
        inline bool operator>(const DateTime &at) const
        {
            if(year > at.year)
                return true;
            else if(year == at.year)
            {
                if(month > at.month)
                    return true;
                else if(month == at.month)
                {
                    if(day > at.day)
                        return true;
                    else if(day == at.day)
                    {
                        if(hour > at.hour)
                            return true;
                        else if(hour == at.hour)
                        {
                            if(minute > at.minute)
                                return true;
                            else
                                return second > at.second;
                        }
                    }
                }
            }
            return false;
        }
        inline bool operator>=(const DateTime &at) const
        {
            return *this == at || *this > at;
        }
        inline bool operator<(const DateTime &at) const
        {
            if(*this == at)
                return false;
            else
                return !(*this > at);
        }
        inline bool operator<=(const DateTime &at) const
        {
            if(*this == at)
                return true;
            else
                return !(*this > at);
        }
        static bool comparator(DateTime at1, DateTime at2) { return at1 <= at2; }
    };
}
    
#endif	/* DATETIME_H */

