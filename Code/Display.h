/*                       ____               ____________
 *                      |    |             |            |
 *                      |    |             |    ________|
 *                      |    |             |   |
 *                      |    |             |   |    
 *                      |    |             |   |    ____
 *                      |    |             |   |   |    |
 *                      |    |_______      |   |___|    |
 *                      |            |  _  |            |
 *                      |____________| |_| |____________|
 *                        
 *      Author(s):      limpygnome (Marcus Craske)              limpygnome@gmail.com
 * 
 *      License:        Creative Commons Attribution-ShareAlike 3.0 Unported
 *                      http://creativecommons.org/licenses/by-sa/3.0/
 * 
 *      File:           Display.h
 *      Path:           BC/Services/Display.h
 * 
 *      Change-Log:
 *                      2013-06-25      Added header.
 * 
 * *****************************************************************************
 * The display service, used for interacting with the shift-registers of the
 * binary clock for displaying light sequences (refer to sequence enum).
 * *****************************************************************************
 */
#ifndef DISPLAY_H
#define	DISPLAY_H

#include "Definitions.h"

#include <unistd.h>
#include <time.h>

#include "IService.h"
using BC::Services::IService;

#include "LightSeqs/LightSequence.h"
#include "LightSeqs/All.h"
#include "LightSeqs/Failure.h"
#include "LightSeqs/Offline.h"
#include "LightSeqs/SingleLed.h"
#include "LightSeqs/SymbolX.h"
#include "LightSeqs/LedTime.h"

using namespace BC::Services::LightSeqs;

#include "Utils.h"
using BC::Utils;

namespace BC
{
    namespace Services
    {
        class Display : public IService
        {
        public:
            // Enums ---------------------------------------------------------->
            static int const NUMBER_OF_SEQUENCES = 7;
            enum Sequence // WARNING: Keep from 0 to numberOfSequences-1 - iterated in Templates.cpp!
            {
                Failure = 0,
                Offline = 1,
                SingleLedTest = 2,
                Time = 3,
                ManualBuffer = 4,
                SymbolX = 5,
                AllLeds = 6
            };
        private:
            // Fields --------------------------------------------------------->
            // -- The display sequences used for the low, high and alarm-buzz
            // -- states.
            Sequence    sequenceHigh,
                        sequenceLow,
                        sequenceBuzz;
                // -- The buffer written to the shift-registers for the display;
                // -- this is for manual-mode.
            int buffer,
                // -- The time periods between writing the buffer to the
                // -- shift-registers in manual-mode.
                manualDelay,
                // -- The light threshold; when surpassed, the high sequence is
                // -- displayed, else the low sequence is displayed.
                lightThreshold,
                // -- GPIO pin used for the shift-register data pin.
                pinData,
                // -- GPIO pin used for the shift-register latch pin.
                pinLatch,
                // -- GPIO pin used for the shift-register clock pin.
                pinClock,
                // -- The number of shift-registers.
                registerCount;
        public:
            // Constructors --------------------------------------------------->
            Display(ServiceController * const controller) : IService(controller), pinClock(DISPLAY_DEFAULT_PIN_CLOCK), pinData(DISPLAY_DEFAULT_PIN_DATA), pinLatch(DISPLAY_DEFAULT_PIN_LATCH), registerCount(DISPLAY_DEFAULT_REGISTERS), lightThreshold(DISPLAY_DEFAULT_LIGHT_THRESHOLD), sequenceLow(DISPLAY_DEFAULT_SEQUENCE_LIGHT_LOW), sequenceHigh(DISPLAY_DEFAULT_SEQUENCE_LIGHT_HIGH), sequenceBuzz(DISPLAY_DEFAULT_SEQUENCE_LIGHT_BUZZ), buffer(0), manualDelay(0) {}
            // Member Functions ----------------------------------------------->
            void run();
            // Member Functions - Mutators ------------------------------------>
            void changeManualBuffer(int value);
            void changeManualDelay(int delay);
            // Member Functions - Accessors ----------------------------------->
            inline Sequence getSequenceHigh() { return sequenceHigh; }
            inline Sequence getSequenceLow() { return sequenceLow; }
            inline Sequence getSequenceBuzz() { return sequenceBuzz; }
            inline int getLightThreshold() { return lightThreshold; }
        private:
            LightSequence* seqCreate(int mode);
            // Member Functions - Light Sequences ----------------------------->
            int sequence_SingleLedTest();
            int sequence_Time();
            int sequence_ManualBuffer();
            int sequence_SymbolX();
            int sequence_Failure();
            int sequence_AllLeds();
        public:
            string getTitle() { return SERVICETITLE_DISPLAY; }
        };
    }
}

#endif	/* DISPLAY_H */

