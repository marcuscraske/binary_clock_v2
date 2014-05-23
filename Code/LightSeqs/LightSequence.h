/* 
 * File:   LightSequence.h
 * Author: limpygnome
 *
 * Created on 23 May 2014, 18:54
 */

#ifndef LIGHTSEQUENCE_H
#define	LIGHTSEQUENCE_H

#include "../Definitions.h"
#include "../ServiceController.h"

namespace BC
{
    namespace Services
    {
        namespace LightSeqs
        {
            class LightSequence
            {
            private:
                ServiceController   *const controller;
                long long           delay;
            protected:
                LightSequence(ServiceController *const controller, long long delay) : controller(controller), delay(delay) {}
            public:
                // Fetches the delay between ticks/cycles of updating the display.
                inline long long getDelay() { return delay; }
                // Invoked every tick/cycle of updating the display; returns
                // the value to be used for the display.
                virtual long updateDisplay() {}
            };
        }
    }
}

#endif	/* LIGHTSEQUENCE_H */
