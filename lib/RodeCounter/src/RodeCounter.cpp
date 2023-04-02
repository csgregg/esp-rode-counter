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


// Project Libraries
#include "RodeCounter.h"
#include "Logger.h"
#include "ConfigManager.h"
#include "IndexPage.h"


////////////////////////////////////////////
//// RodeSettings Class

// Public:

// Resets rode settings to default
void ICACHE_FLASH_ATTR RodeSettings::SetDefaults() {

    windlassDiameter = DEFAULT_WINDLASS_DIAMETER;       // The diameter of the windlass (mm)
    windlassSpeed = DEFAULT_WINDLASS_SPEED;             // The speed of the windlass (rpm)
    windlassReversed = DEFAULT_WINDLASS_REVERSED;       // Are the windlass controls reversed
    chainLength = DEFAULT_CHAIN_LENGTH;                 // Overall length of the chain (mm)
    waterLine = DEFAULT_WATER_LINE;                     // The water line on the chain (mm)
}


////////////////////////////////////////////
//// RodeCounter Class

// Public:

// Initializes the rode counter
void ICACHE_FLASH_ATTR RodeCounter::Begin( RodeSettings& settings ) {

    LOG( PSTR("(Rode) Starting rode counter") );

    _settings = &settings;

    LoadRodeSettings();

    _time = millis();           // TODO - Remove

    // Start up the hardware pins for debounce, etc
    _upInput.Begin();
    _downInput.Begin();
    _sensorInput.Begin();

}


// Loads rode counter settings
void ICACHE_FLASH_ATTR RodeCounter::LoadRodeSettings() {

    // Convertions from mm to cm needed for each
    indexpage.water_line.setValue(_settings->waterLine/10);
    indexpage.rode_len.setValue(_settings->chainLength/10);
    indexpage.warn_limit_1.setValue((_settings->waterLine + 500)/10);        // Water line + 5m
    indexpage.warn_limit_2.setValue((_settings->chainLength - 700)/10);        // Length - 7m
    indexpage.warn_limit_3.setValue((_settings->chainLength - 200)/10);        // Length - 2m
   
    ResetRode();

}


// Handles any repeasting rode counting tasks
void ICACHE_FLASH_ATTR RodeCounter::Handle() {

    // Check if chain has started moving up and update GUI
    if( _upInput.IsChanged() != HardwareSwitch::INACTIVE ) {
        _upInput.ResetTrigger();
        _chainDirection = _upInput.GetState() ? ( _settings->windlassReversed ? Direction::DOWN : Direction::UP ) : Direction::STOPPED;
        indexpage.UpdateWindlassStatus();
    }

    // Check if chain has started moving down and update GUI
    if( _downInput.IsChanged() != HardwareSwitch::INACTIVE ) {
        _downInput.ResetTrigger();
        _chainDirection = _downInput.GetState() ? ( _settings->windlassReversed ? Direction::UP : Direction::DOWN ) : Direction::STOPPED;
        indexpage.UpdateWindlassStatus();
    }

    // Check if the sensor has triggered rising or falling
    if( _sensorInput.IsChanged() != HardwareSwitch::INACTIVE ) {

        _sensorInput.ResetTrigger();

        // Only count down on sensor going active - don't need to worry if windlass is reversed
        if( _sensorInput.IsChanged() == HardwareSwitch::GOING_ACTIVE && _chainDirection == DOWN ) {
            _currentRode += _settings->windlassDiameter;
            indexpage.UpdateWindlassStatus();
        }

        // Only count uo on sensor going inactive
        if( _sensorInput.IsChanged() == HardwareSwitch::GOING_INACTIVE && _chainDirection == UP ) {
            _currentRode -= _settings->windlassDiameter;
            indexpage.UpdateWindlassStatus();
        }
    }


    // TODO - remove section
    if( millis() > _time + 1000 ) {
        _time = millis();
        
        if( _chainDirection == DOWN ) {
            _currentRode += _settings->windlassDiameter;
            indexpage.UpdateWindlassStatus();
        }

        delay(100);

        if( _chainDirection == UP ) {
            _currentRode -= _settings->windlassDiameter;
            indexpage.UpdateWindlassStatus();
        }
        
        if( _currentRode >= _settings->chainLength || _currentRode <= 0 ) {
            _chainDirection = _chainDirection == UP ? DOWN : UP;
            DEBUG("Reverse");
        }
    }

}


// Protected:


RodeCounter rodecounter;         // Create the global instance