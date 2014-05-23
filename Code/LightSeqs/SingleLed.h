/* 
 * File:   SingleLed.h
 * Author: limpygnome
 *
 * Created on 23 May 2014, 19:28
 */

#ifndef SINGLELED_H
#define	SINGLELED_H

#include "LightSequence.h"

namespace BC
{
    namespace Services
    {
        namespace LightSeqs
        {
            class SingleLed : public LightSequence
            {
            private:
                long long buffer;
            public:
                SingleLed(ServiceController *const controller) : LightSequence(controller, 350), buffer(0) {}
                
                long updateDisplay()
                {
                    if(buffer >= 16777216)
                        buffer = 0;
                    else
                        buffer = (buffer << 1) | 0x1;
                    return buffer;
                }
            };
        }
    }
}

#endif	/* SINGLELED_H */

