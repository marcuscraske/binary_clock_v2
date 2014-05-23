/* 
 * File:   SymbolX.h
 * Author: limpygnome
 *
 * Created on 23 May 2014, 19:16
 */

#ifndef SYMBOLX_H
#define	SYMBOLX_H

#include "LightSequence.h"

namespace BC
{
    namespace Services
    {
        namespace LightSeqs
        {
            class SymbolX : public LightSequence
            {
            public:
                SymbolX(ServiceController *const controller) : LightSequence(controller, 1000) {}
                
                long updateDisplay()
                {
                    return DISPLAY_LED_H_8 + DISPLAY_LED_H_1 + DISPLAY_LED_M_4 + DISPLAY_LED_M_2 + DISPLAY_LED_S_8 + DISPLAY_LED_S_1;
                }
            };
        }
    }
}

#endif	/* SYMBOLX_H */

