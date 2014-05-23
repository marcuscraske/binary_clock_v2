/* 
 * File:   Time.h
 * Author: limpygnome
 *
 * Created on 23 May 2014, 19:20
 */

#ifndef LEDTIME_H
#define	LEDTIME_H

#include "LightSequence.h"

namespace BC
{
    namespace Services
    {
        namespace LightSeqs
        {
            class LedTime : public LightSequence
            {
            public:
                LedTime(ServiceController *const controller) : LightSequence(controller, 1000) {}
                
                long updateDisplay()
                {
                    long long buffer = 0;
                    // Fetch the current time
                    BC::Time time = Utils::getTime();
                    buffer = 0;
                    // Build the buffer value to represent the time
                    // -- Hour
                    if((time.hour & 16) > 0)
                        buffer += DISPLAY_LED_H_16;
                    if((time.hour & 8) > 0)
                        buffer += DISPLAY_LED_H_8;
                    if((time.hour & 4) > 0)
                        buffer += DISPLAY_LED_H_4;
                    if((time.hour & 2) > 0)
                        buffer += DISPLAY_LED_H_2;
                    if((time.hour & 1) > 0)
                        buffer += DISPLAY_LED_H_1;
                    // -- Minute
                    if((time.minute & 32) > 0)
                        buffer += DISPLAY_LED_M_32;
                    if((time.minute & 16) > 0)
                        buffer += DISPLAY_LED_M_16;
                    if((time.minute & 8) > 0)
                        buffer += DISPLAY_LED_M_8;
                    if((time.minute & 4) > 0)
                        buffer += DISPLAY_LED_M_4;
                    if((time.minute & 2) > 0)
                        buffer += DISPLAY_LED_M_2;
                    if((time.minute & 1) > 0)
                        buffer += DISPLAY_LED_M_1;
                    // -- Second
                    if((time.second & 32) > 0)
                        buffer += DISPLAY_LED_S_32;
                    if((time.second & 16) > 0)
                        buffer += DISPLAY_LED_S_16;
                    if((time.second & 8) > 0)
                        buffer += DISPLAY_LED_S_8;
                    if((time.second & 4) > 0)
                        buffer += DISPLAY_LED_S_4;
                    if((time.second & 2) > 0)
                        buffer += DISPLAY_LED_S_2;
                    if((time.second & 1) > 0)
                        buffer += DISPLAY_LED_S_1;
                    return buffer;
                }
            };
        }
    }
}

#endif	/* LEDTIME_H */

