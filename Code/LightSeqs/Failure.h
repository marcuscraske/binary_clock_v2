/* 
 * File:   Failure.h
 * Author: limpygnome
 *
 * Created on 23 May 2014, 19:10
 */

#ifndef FAILURE_H
#define	FAILURE_H

#include "LightSequence.h"

namespace BC
{
    namespace Services
    {
        namespace LightSeqs
        {
            class Failure : public LightSequence
            {
            private:
                bool on;
            public:
                Failure(ServiceController *const controller) : LightSequence(controller, 50), on(false) {}
                
                long updateDisplay()
                {
                    on = !on;
                    return on ? DISPLAY_LED_ERROR_1 + DISPLAY_LED_ERROR_2 + DISPLAY_LED_ERROR_3 : 0;
                }
            };
        }
    }
}

#endif	/* FAILURE_H */

