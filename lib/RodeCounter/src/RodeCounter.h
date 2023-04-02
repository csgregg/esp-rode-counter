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
    #include "Device.h"
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
            uint chainLength;               // Overall length of the chain (mm)
            uint waterLine;                 // The water line on the chain (mm)

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

            // Chain movement
            enum Direction : uint {
                STOPPED = 0,
                UP = 1,
                DOWN = 2
            };

            // Constructor
            ICACHE_FLASH_ATTR RodeCounter() : 
                _sensorInput(D2, HardwareSwitch::ACTIVE_LOW, CHANGE),      // Setup the hardware pins
                _upInput(D3, HardwareSwitch::ACTIVE_LOW, CHANGE),
                _downInput(D4, HardwareSwitch::ACTIVE_LOW, CHANGE)
            {}

            /** Starts the rode counter
             * @param settings      Reference of rode settings struct */
            void ICACHE_FLASH_ATTR Begin( RodeSettings& settings );

            /** Handles any repeated rode counting tasks */
            void ICACHE_FLASH_ATTR Handle();

            /** Resets the current rode deployed to zero and ready to go down - should only happen when anchor is completely raised and stowed */
            void ICACHE_FLASH_ATTR ResetRode() { _currentRode = 0; _chainDirection = DOWN; };

            /** Is the chain going up */
            Direction ICACHE_FLASH_ATTR GetChainDirection(){ return _chainDirection; };

            /** Get the current deployed rode */
            uint ICACHE_FLASH_ATTR GetCurrentRode(){ return _currentRode; };

            /** Load rode counter settings */
            void ICACHE_FLASH_ATTR LoadRodeSettings();

        protected:

            RodeSettings* _settings;        // Pointer to data struct containing rode settings

            Direction _chainDirection;       // What is the chain doing
            uint _currentRode = 0;           // Current amount of rode deployed (mm)

            uint _time;       // TODO - Remove

            // Hardware pins
            HardwareSwitch _sensorInput;
            HardwareSwitch _upInput;
            HardwareSwitch _downInput;

    };

    extern RodeCounter rodecounter;         // Global instance of the rode counter services

#endif      // RODE_COUNTER_H