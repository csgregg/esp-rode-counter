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

    windlassDiameter = 300;             // The diameter of the windlass (mm)
    windlassSpeed = 75;                 // The speed of the windlass (rpm)
    windlassReversed = false;           // Are the windlass controls reversed
    chainLength = 5000;                 // Overall length of the chain (cm)
    waterLine = 200;                    // The water line on the chain (cm)
}


// Initializes the rode counter
void ICACHE_FLASH_ATTR RodeCounter::Begin( RodeSettings& settings ) {

    LOG( PSTR("(Rode) Starting rode counter") );

    _settings = &settings;

    _currentRode = _settings->currentRode;

    indexpage.water_line.setValue(_settings->waterLine);
    indexpage.rode_len.setValue(_settings->chainLength);
    indexpage.warn_limit_1.setValue(_settings->waterLine + 500);        // Water line + 5m
    indexpage.warn_limit_2.setValue(_settings->chainLength - 700);        // Length - 7m
    indexpage.warn_limit_3.setValue(_settings->chainLength - 200);        // Length - 2m

    _currentRode = 0;
    _chainDown = false;
    _chainUp = true;

    _time = millis();
}


// Handles any repeasting rode counting tasks
void RodeCounter::Handle() {

    if( millis() > _time + 1000 ) {
        _time = millis();
        _currentRode += ( _chainDown ? 100 : -100 );
        if( _currentRode > _settings->chainLength || _currentRode < 0 ) {
            _chainDown = !_chainDown;
            _chainUp = !_chainUp;
            DEBUG("Reverse");
        }
    }

    indexpage.chain_up.setValue(_chainUp);
    indexpage.chain_down.setValue(_chainDown);
    indexpage.current_rode.setValue(_currentRode);

}


// Resets the current rode deployed to zero
void ICACHE_FLASH_ATTR RodeCounter::ResetToZero() {

    LOG( PSTR("(Rode) Resetting rode to zero") );

    _currentRode = 0;

    StoreCurrentRode();
}


// Protected:

// Stores the current rode to memory
void ICACHE_FLASH_ATTR RodeCounter::StoreCurrentRode() {

    LOG( PSTR("(Rode) Saving current rode to memory") );

    config.settings.rodeSettings.currentRode = _currentRode;
    config.Save();

}


RodeCounter rodecounter;         // Create the global instance