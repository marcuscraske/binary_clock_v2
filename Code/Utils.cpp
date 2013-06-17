
#include "Utils.h"

namespace BC
{
    DateTime Utils::getDateTime()
    {
        // Get the raw time
        time_t raw;
        time(&raw);
        // Convert to our struct
        struct tm *time;
        time = localtime(&raw);
        // -- tm_year (years since 1990)
        return DateTime(1900 + time->tm_year, time->tm_mon+1, time->tm_mday, time->tm_hour, time->tm_min, time->tm_sec);
    }
    Time Utils::getTime()
    {
        // Get the raw time
        time_t raw;
        time(&raw);
        // Convert to our struct
        struct tm *time;
        time = localtime(&raw);
        // -- tm_year (years since 1990)
        return Time(time->tm_hour, time->tm_min, time->tm_sec);
    }
    string Utils::getTimeDateGBString()
    {
        DateTime t = getDateTime();
        return std::to_string(t.day) + "/" + std::to_string(t.month) + "/" + std::to_string(t.year) + " " + std::to_string(t.hour) + ":" + std::to_string(t.minute) + ":" + std::to_string(t.second);
    }
}