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
//// Global Functions

// Interrupt service routine for pulse pin
int pulseCounter = 0;       // Count of windlass rotations
bool revsersed = false;     // Is the windlass reversed
uint maxPulse = 0;

void ICACHE_RAM_ATTR pulseInterrupt() {

    if( digitalRead( PULSE_PIN ) == LOW && digitalRead( UP_PIN ) == LOW ) pulseCounter -= (revsersed ? -1 : 1);      // Active rising pulse - up
    else if( digitalRead( PULSE_PIN ) == HIGH && digitalRead( DOWN_PIN ) == LOW ) pulseCounter += (revsersed ? -1 : 1);   // Active falling pulse - down

    if( pulseCounter < 0 ) pulseCounter = 0;
    if( pulseCounter > (int)maxPulse ) pulseCounter = maxPulse;
}


////////////////////////////////////////////
//// RodeSettings Class

// Public:

// Resets rode settings to default
void ICACHE_FLASH_ATTR RodeSettings::SetDefaults() {

    windlassDiameter = DEFAULT_WINDLASS_DIAMETER;       // The diameter of the windlass (mm)
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

    // Pulse pin
    pinMode( PULSE_PIN, INPUT_PULLUP );
    attachInterrupt( PULSE_PIN, pulseInterrupt, CHANGE ) ;

    // Direction pins
    pinMode( UP_PIN, INPUT_PULLUP );    // Up
    pinMode( DOWN_PIN, INPUT_PULLUP );    // Down

    delay(2000);

    ResetRode();

}


// Loads rode counter settings
void ICACHE_FLASH_ATTR RodeCounter::LoadRodeSettings() {

    // Convertions from mm to cm needed for each
    indexpage.water_line.setValue(_settings->waterLine/10);
    indexpage.rode_len.setValue(_settings->chainLength/10);
    indexpage.warn_limit_1.setValue((_settings->waterLine + 5000)/10);           // Water line + 5m
    indexpage.warn_limit_2.setValue((_settings->chainLength - 7000)/10);         // Length - 7m
    indexpage.warn_limit_3.setValue((_settings->chainLength - 2000)/10);         // Length - 2m

    revsersed = _settings->windlassReversed;
    maxPulse = _settings->chainLength / _settings->windlassDiameter;
}


// Handles any repeasting rode counting tasks
void ICACHE_FLASH_ATTR RodeCounter::Handle() {

    // Set direction
    _chainDirection = Direction::STOPPED;
    if( digitalRead( UP_PIN ) == LOW ) _chainDirection = revsersed ? Direction::DOWN : Direction::UP;
    if( digitalRead( DOWN_PIN ) == LOW ) _chainDirection = revsersed ? Direction::UP : Direction::DOWN;

    // Calculate length
    _currentRode = pulseCounter * _settings->windlassDiameter;       

    indexpage.UpdateWindlassStatus();

}


// Resets the current rode deployed to zero and ready to go down - should only happen when anchor is completely raised and stowed */
void ICACHE_FLASH_ATTR RodeCounter::ResetRode() {
    _chainDirection = STOPPED;
    pulseCounter = 0;
};


// Protected:


RodeCounter rodecounter;         // Create the global instance