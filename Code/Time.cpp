
#include "Time.h"

namespace BC
{
    bool Time::parse(string text, Time &dest)
    {
        Time t;
        vector<string> tokens = Utils::split(text, ':');
        if(tokens.size() == 3 && Utils::tryParse(tokens.at(0), t.hour) && Utils::tryParse(tokens.at(1), t.minute) && Utils::tryParse(tokens.at(2), t.second))
        {
            dest = t;
            return true;
        }
        else
            return false;
    }
}
