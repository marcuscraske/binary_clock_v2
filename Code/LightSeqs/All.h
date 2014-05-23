/* 
 * File:   All.h
 * Author: limpygnome
 *
 * Created on 23 May 2014, 19:04
 */

#ifndef ALL_H
#define	ALL_H

#include "LightSequence.h"

namespace BC
{
    namespace Services
    {
        namespace LightSeqs
        {
            class All : public LightSequence
            {
            public:
                All(ServiceController *const controller) : LightSequence(controller, 1000) {}
                
                long updateDisplay()
                {
                    return DISPLAY_LED_H_1 + DISPLAY_LED_H_2 + DISPLAY_LED_H_4 + DISPLAY_LED_H_8 + DISPLAY_LED_H_16 +
                    DISPLAY_LED_M_1 + DISPLAY_LED_M_2 + DISPLAY_LED_M_4 + DISPLAY_LED_M_8 + DISPLAY_LED_M_16 + DISPLAY_LED_M_32 +
                    DISPLAY_LED_S_1 + DISPLAY_LED_S_2 + DISPLAY_LED_S_4 + DISPLAY_LED_S_8 + DISPLAY_LED_S_16 + DISPLAY_LED_S_32;
                }
            };
        }
    }
}

#endif	/* ALL_H */

