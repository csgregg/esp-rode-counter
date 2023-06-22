/**
 * @file        Device.cpp
 * @author      Chris Gregg
 * 
 * @brief       Defines the physical attributes of the IOT device and the build environment.
 *              Build flags are loaded from platformio.ini
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


// Global Libraries
#include <DoubleResetDetector.h>

// Project Libraries
#include "Device.h"

// Global Objects
DoubleResetDetector drd( DRD_TIMEOUT, DRD_ADDRESS );


////////////////////////////////////////////
//// Device Manager Class

// Public:


bool ICACHE_FLASH_ATTR DebouncedInput::Begin() {

    pinMode( _pin, _type == PinType::ACTIVE_HIGH ? INPUT : INPUT_PULLUP );

    _firstTriggerTime = 0;
    _bouncing = false;
    _triggercount = 0;

    if( _function == PinFunction::LEVEL) return true;

    // Only do the rest if it is an interrupt pin

    int8_t irq = digitalPinToInterrupt(_pin);
    
    if (irq != NOT_AN_INTERRUPT)
    {

        // assign ourselves a ISR ID ...
        _myISRId = UINT8_MAX;
        for (uint8_t i = 0; i < MAX_ISR; i++) {
            if (!(_ISRUsed & _BV(i))) {         // found a free ISR Id? 
                _myISRId = i;                   // remember who this instance is
                _myInstance[_myISRId] = this;   // record this instance
                _ISRUsed |= _BV(_myISRId);      // lock this in the allocations table
                break;
            }
        }

        // ... and attach corresponding ISR callback from the lookup table
        static void((*ISRfunc[MAX_ISR])()) =
        {
            // Declare all the [MAX_ISR] encoder ISRs
            #define GISRM2(i, _) CAT(globalISR,i), 
            EVAL(REPEAT( MAX_ISR, GISRM2, ~))
        };
  
        ResetTrigger();

        if (_myISRId != UINT8_MAX) {
          if( _trigger == CHANGE ) attachInterrupt(irq, ISRfunc[_myISRId], CHANGE );
          else if( _type == PinType::ACTIVE_HIGH ) attachInterrupt(irq, ISRfunc[_myISRId], ( (_type == PinType::ACTIVE_HIGH) != (_trigger == FALLING) ) ? RISING : FALLING ) ;
        } else irq = NOT_AN_INTERRUPT;

    }
    return(irq != NOT_AN_INTERRUPT);
}


// Instance ISR handler called from static ISR globalISRx
void ICACHE_RAM_ATTR DebouncedInput::instanceISR() { 
    
    if( _function == PinFunction::INTERRUPT ) {

        // No debounce

        _triggercount++;

        if( _trigger == CHANGE ) _change = (_type == PinType::ACTIVE_HIGH) != (digitalRead( _pin ) == LOW) ? ActiveChange::GOING_ACTIVE : ActiveChange::GOING_INACTIVE;
        else _change = ( (_type == PinType::ACTIVE_HIGH) != (_trigger == FALLING) ? RISING : FALLING ) ? ActiveChange::GOING_ACTIVE : ActiveChange::GOING_INACTIVE;

    } else {

        // Debouncing

        if( _bouncing && millis() - _firstTriggerTime > DEBOUNCE_TIME ) {

            _firstTriggerTime = 0;
            _bouncing = false;
            _triggercount++;

            if( _trigger == CHANGE ) _change = (_type == PinType::ACTIVE_HIGH) != (digitalRead( _pin ) == LOW) ? ActiveChange::GOING_ACTIVE : ActiveChange::GOING_INACTIVE;
            else _change = ( (_type == PinType::ACTIVE_HIGH) != (_trigger == FALLING) ? RISING : FALLING ) ? ActiveChange::GOING_ACTIVE : ActiveChange::GOING_INACTIVE;
        }

        // Only on first trigger
        if( _firstTriggerTime == 0 ) {
            _firstTriggerTime = millis();
            _bouncing = true;
        }

    }

}   



// Interrupt handling declarations required outside the class
uint8_t DebouncedInput::_ISRUsed = 0;           // allocation table for the globalISRx()
DebouncedInput* DebouncedInput::_myInstance[MAX_ISR]; // callback instance handle for the ISR
 
// ISR for each _myISRId
#define GISRM3(i, _) void ICACHE_RAM_ATTR DebouncedInput::CAT(globalISR,i)(){ DebouncedInput::_myInstance[i]->instanceISR(); };
EVAL(REPEAT( MAX_ISR, GISRM3, ~))


////////////////////////////////////////////
//// Device Manager Class

// Public:

// Sets up the device hardware and build environment
void ICACHE_FLASH_ATTR Device::Begin() {

    // Need to do this because these preprocessor defines seem to get defined at differnet time to the rest
    sprintf_P( _buildNo, PSTR("%i"), flag_BUILD_NO );
    strcpy_P( _buildTime, flag_BUILD_TIMESTAMP );
    sprintf_P( _chipID, PSTR("%0X") ,EspClass::getChipId() );
    strcpy_P( _buildEnv, flag_BUILD_ENV );

    if( drd.detectDoubleReset() ) _startMode = DOUBLERESET;
    
    // Physical IO Setup

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN,HIGH);
}


// Handles any repeating tasks
void ICACHE_FLASH_ATTR Device::Handle(){
    drd.loop();       // Handle double reset detection
}


Device device;           // Create the global instance

