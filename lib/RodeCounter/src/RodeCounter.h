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

    // Global Libraries
    #include <Arduino.h>

    // Project Libraries
    #include "NetworkManager.h"
    #include "TimeLocation.h"

    /** @class RodeSettings
     *  @brief Data struct class to store the rode settings */
    class RodeSettings {

        public:

            /** Restests settings to defaults */
            void ICACHE_FLASH_ATTR SetDefaults();

            uint windlassDiameter;          // The diameter of the windlass (mm)
            uint windlassSpeed;             // The speed of the windlass (rpm)
            bool windlassReversed;          // Are the windlass controls reversed
            uint chainLength;               // Overall length of the chain (cm)
            uint waterLine;                 // The water line on the chain (cm)

             // Create a compare operators

            bool operator== ( const RodeSettings& other ) const {
                return windlassDiameter == other.windlassDiameter
                    && windlassSpeed == other.windlassSpeed
                    && windlassReversed == other.windlassReversed
                    && chainLength == other.chainLength
                    && waterLine == other.waterLine;
            }
            bool operator!= ( const RodeSettings& other ) const {
                return windlassDiameter != other.windlassDiameter
                    || windlassSpeed != other.windlassSpeed
                    || windlassReversed != other.windlassReversed
                    || chainLength != other.chainLength
                    || waterLine != other.waterLine;
            }
    };


    /** @class RodeCounter
     *  @brief Manages the rode counting */
    class RodeCounter {
                
            public:

            /** Starts the rode counter
             * @param settings      Reference of rode settings struct */
            void ICACHE_FLASH_ATTR Begin( RodeSettings& settings );

            /** Handles any repeated rode counting tasks */
            void Handle();

            /** Resets the current rode deployed to zero */
            void ResetRodeToZero() { _currentRode = 0; };

            /** Is the chain going up */
            bool isChainUp(){ return _chainUp; };

            /** Is the chain going down */
            bool isChainDown(){ return _chainDown; };

            /** Get the current deployed rode */
            uint getCurrentRode(){ return _currentRode; };

            /** Load rode counter settings */
            void LoadRodeSettings();

        protected:

            RodeSettings* _settings;        // Pointer to data struct containing rode settings

            bool _chainUp = false;           // Is the chain traveling up
            bool _chainDown = false;         // Is the chain traveling down 

            uint _currentRode = 0;           // Current amount of rode deployed (cm)

            void ICACHE_FLASH_ATTR StoreCurrentRode();      // Stores the current rode to memory

            uint _time;

    };

    extern RodeCounter rodecounter;         // Global instance of the rode counter services

#endif      // RODE_COUNTER_H