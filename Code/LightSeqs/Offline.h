/* 
 * File:   Offline.h
 * Author: limpygnome
 *
 * Created on 23 May 2014, 19:29
 */

#ifndef OFFLINE_H
#define	OFFLINE_H

#include "LightSequence.h"

namespace BC
{
    namespace Services
    {
        namespace LightSeqs
        {
            class Offline : public LightSequence
            {
            private:
                int row;
            public:
                Offline(ServiceController *const controller) : LightSequence(controller, 1000), row(0) {}
                
                long updateDisplay()
                {
                    long long value;
                    switch(row)
                    {
                        case 0:
                            value =
                                    DISPLAY_LED_M_32 + DISPLAY_LED_S_32 +
                                    DISPLAY_LED_H_16 + DISPLAY_LED_M_16 + DISPLAY_LED_S_16 +
                                    DISPLAY_LED_H_8 + DISPLAY_LED_M_8 + DISPLAY_LED_S_8 +
                                    DISPLAY_LED_H_4 + DISPLAY_LED_M_4 + DISPLAY_LED_S_4 +
                                    DISPLAY_LED_H_2 + DISPLAY_LED_M_2 + DISPLAY_LED_S_2 +
                                    DISPLAY_LED_H_1 + DISPLAY_LED_M_1 + DISPLAY_LED_S_1;
                            break;
                        case 1:
                            value =
                                    DISPLAY_LED_H_16 + DISPLAY_LED_M_16 + DISPLAY_LED_S_16 +
                                    DISPLAY_LED_H_8 + DISPLAY_LED_M_8 + DISPLAY_LED_S_8 +
                                    DISPLAY_LED_H_4 + DISPLAY_LED_M_4 + DISPLAY_LED_S_4 +
                                    DISPLAY_LED_H_2 + DISPLAY_LED_M_2 + DISPLAY_LED_S_2 +
                                    DISPLAY_LED_H_1 + DISPLAY_LED_M_1 + DISPLAY_LED_S_1;
                            break;
                        case 2:
                            value =
                                    DISPLAY_LED_H_8 + DISPLAY_LED_M_8 + DISPLAY_LED_S_8 +
                                    DISPLAY_LED_H_4 + DISPLAY_LED_M_4 + DISPLAY_LED_S_4 +
                                    DISPLAY_LED_H_2 + DISPLAY_LED_M_2 + DISPLAY_LED_S_2 +
                                    DISPLAY_LED_H_1 + DISPLAY_LED_M_1 + DISPLAY_LED_S_1;
                            break;
                        case 3:
                            value =
                                    DISPLAY_LED_H_4 + DISPLAY_LED_M_4 + DISPLAY_LED_S_4 +
                                    DISPLAY_LED_H_2 + DISPLAY_LED_M_2 + DISPLAY_LED_S_2 +
                                    DISPLAY_LED_H_1 + DISPLAY_LED_M_1 + DISPLAY_LED_S_1;
                            break;
                        case 4:
                            value =
                                    DISPLAY_LED_H_2 + DISPLAY_LED_M_2 + DISPLAY_LED_S_2 +
                                    DISPLAY_LED_H_1 + DISPLAY_LED_M_1 + DISPLAY_LED_S_1;
                            break;
                        case 5:
                            value =
                                    DISPLAY_LED_H_1 + DISPLAY_LED_M_1 + DISPLAY_LED_S_1;
                            break;
                        case 6:
                            value = 0;
                            break;
                    }
                    if(row != 6)
                        row++;
                    return value;
                }
            };
        }
    }
}

#endif	/* OFFLINE_H */

