/**
 * @file        RodeCounter.h
 * @author      Chris Gregg
 * 
 * @brief       Implements an anchor chain rode counter
 * 
 * @copyright   Copyright (c) 2023
 * 
 */

/* MIT License

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. */


#ifndef RODE_COUNTER_H

    #define RODE_COUNTER_H


    // Define hardware pins
    #define PULSE_PIN D5
    #define UP_PIN D6
    #define DOWN_PIN D7

    // Global Libraries
    #include <Arduino.h>

    // Project Libraries
    #include "Device.h"
    #include "NetworkManager.h"


    #define DEFAULT_WINDLASS_DIAMETER 300;             // The diameter of the windlass (mm)
    #define DEFAULT_WINDLASS_REVERSED false;           // Are the windlass controls reversed
    #define DEFAULT_CHAIN_LENGTH 50000;                // Overall length of the chain (mm)
    #define DEFAULT_WATER_LINE 2000;                   // The water line on the chain (mm)


    /** @class RodeSettings
     *  @brief Data struct class to store the rode settings */
    class RodeSettings {

        public:

            /** Restests settings to defaults */
            void ICACHE_FLASH_ATTR SetDefaults();

            uint windlassDiameter;          // The diameter of the windlass (mm)
            bool windlassReversed;          // Are the windlass controls reversed
            uint chainLength;               // Overall length of the chain (mm)
            uint waterLine;                 // The water line on the chain (mm)

             // Create a compare operators

            bool operator== ( const RodeSettings& other ) const {
                return windlassDiameter == other.windlassDiameter
                    && windlassReversed == other.windlassReversed
                    && chainLength == other.chainLength
                    && waterLine == other.waterLine;
            }
            bool operator!= ( const RodeSettings& other ) const {
                return windlassDiameter != other.windlassDiameter
                    || windlassReversed != other.windlassReversed
                    || chainLength != other.chainLength
                    || waterLine != other.waterLine;
            }
    };


    /** @class RodeCounter
     *  @brief Manages the rode counting */
    class RodeCounter {
                
        public:

            // Chain movement
            enum Direction : uint {
                STOPPED = 0,
                UP = 1,
                DOWN = 2
            };

            /** Starts the rode counter
             * @param settings      Reference of rode settings struct */
            void ICACHE_FLASH_ATTR Begin( RodeSettings& settings );

            /** Handles any repeated rode counting tasks */
            void ICACHE_FLASH_ATTR Handle();

            /** Resets the current rode deployed to zero and ready to go down - should only happen when anchor is completely raised and stowed */
            void ICACHE_FLASH_ATTR ResetRode();

            /** Is the chain going up */
            Direction ICACHE_FLASH_ATTR GetChainDirection(){ return _chainDirection; };

            /** Get the current deployed rode */
            uint ICACHE_FLASH_ATTR GetCurrentRode(){ return _currentRode; };

            /** Load rode counter settings */
            void ICACHE_FLASH_ATTR LoadRodeSettings();

        protected:

            RodeSettings* _settings;        // Pointer to data struct containing rode settings

            Direction _chainDirection = STOPPED;        // What is the chain doing
            uint _currentRode = 0;                      // Current amount of rode deployed (mm)

    };

    extern RodeCounter rodecounter;         // Global instance of the rode counter services

#endif      // RODE_COUNTER_H