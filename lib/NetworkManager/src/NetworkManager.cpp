/**
 * @file        NetworkManager.cpp
 * @author      Chris Gregg
 * 
 * @brief       Manages Network Services
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
#include "NetworkManager.h"
#include "Logger.h"
#include "Env.h"


////////////////////////////////////////////
//// Network Settings Class

// Public:

// Resets network settings to default
void ICACHE_FLASH_ATTR NetworkSettings::SetDefaults() {
    wifiSettings.SetDefaults();
}


////////////////////////////////////////////
//// Network Manager Class

// Public:

// Starts the network services
void ICACHE_FLASH_ATTR NetworkManager::Begin( NetworkSettings& settings ) {

    LOG( PSTR("(Network) Starting network services") );

    _settings = &settings;

    // If double reset start in AP mode
    if( device.GetStartMode() == Env::DOUBLERESET ) {
        LOG( PSTR("(Network) Double Reset - starting in AP Mode") );
        strcpy_P( _settings->wifiSettings.apSettings.SSID, flag_DEVICE_CODE );
        strcpy_P( _settings->wifiSettings.apSettings.password, "" );
        _settings->wifiSettings.wifiMode = WIFI_AP;
    }

    // Start all network services
    _wifi.Begin( _settings->wifiSettings );

}



// Handle any repeating network tasks
void NetworkManager::Handle() {
    _wifi.Handle();
}


NetworkManager network;         // Create the global instance
