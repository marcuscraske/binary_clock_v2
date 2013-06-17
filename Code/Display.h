#ifndef DISPLAY_H
#define	DISPLAY_H

// Definitions of shift-register pins for the LEDs
// -- Hours
#define DISPLAY_LED_H_16        4096
#define DISPLAY_LED_H_8         8192
#define DISPLAY_LED_H_4         16384
#define DISPLAY_LED_H_2         32768
#define DISPLAY_LED_H_1         65536
// -- Minutes
#define DISPLAY_LED_M_32        64
#define DISPLAY_LED_M_16        128
#define DISPLAY_LED_M_8         256
#define DISPLAY_LED_M_4         512
#define DISPLAY_LED_M_2         1024
#define DISPLAY_LED_M_1         2048
// -- Seconds
#define DISPLAY_LED_S_32        1
#define DISPLAY_LED_S_16        2
#define DISPLAY_LED_S_8         4
#define DISPLAY_LED_S_4         8
#define DISPLAY_LED_S_2         16
#define DISPLAY_LED_S_1         32
// -- Error LEDs
#define DISPLAY_LED_ERROR_1     131072
#define DISPLAY_LED_ERROR_2     262144
#define DISPLAY_LED_ERROR_3     524288
// Definitions of pin info for interfacing with the shift-registers
#define DISPLAY_DEFAULT_REGISTERS       3
#define DISPLAY_DEFAULT_PIN_DATA        0
#define DISPLAY_DEFAULT_PIN_LATCH       2
#define DISPLAY_DEFAULT_PIN_CLOCK       3
// Definitions of default light sequences
#define DISPLAY_DEFAULT_SEQUENCE_LIGHT_LOW      Sequence::Offline
#define DISPLAY_DEFAULT_SEQUENCE_LIGHT_HIGH     Sequence::Time
#define DISPLAY_DEFAULT_SEQUENCE_LIGHT_BUZZ     Sequence::AllLeds
#define DISPLAY_DEFAULT_LIGHT_THRESHOLD         35

#include <unistd.h>
#include <time.h>

#include "Definitions.h"
#include "IService.h"
using BC::Services::IService;

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
                // -- can be set publicly when in manual-mode.
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
            void changeBuffer(int value);
            void changeManualDelay(int delay);
            // Member Functions - Accessors ----------------------------------->
            inline Sequence getSequenceHigh() { return sequenceHigh; }
            inline Sequence getSequenceLow() { return sequenceLow; }
            inline Sequence getSequenceBuzz() { return sequenceBuzz; }
            inline int getLightThreshold() { return lightThreshold; }
        private:
            // Member Functions - Light Sequences ----------------------------->
            inline int sequence_SingleLedTest();
            inline int sequence_Time();
            inline int sequence_ManualBuffer();
            inline int sequence_SymbolX();
            inline int sequence_Failure();
            inline int sequence_AllLeds();
        public:
            string getTitle() { return SERVICETITLE_DISPLAY; }
        };
    }
}

#endif	/* DISPLAY_H */

