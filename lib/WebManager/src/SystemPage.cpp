/**
 * @file        SystemPage.cpp
 * @author      Chris Gregg
 * 
 * @brief       Server-side functions of system.html
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
#include "SystemPage.h"
#include "Env.h"
#include "Logger.h"
#include "ConfigManager.h"
#include "NetworkManager.h"


////////////////////////////////////////////
//// Sytsem Page Class

// Public:

// Function to initialize AJAX on this page
void ICACHE_FLASH_ATTR SystemPage::InitializeAjax(){

    LOG( PSTR("(Page) System - Initialize AJAX") );

}


// Function to handle AJAX requests for this page
void ICACHE_FLASH_ATTR SystemPage::HandleAjax(){

    LOG( PSTR("(Page) System - Handle AJAX") );

    // Restart device
    if( website.AjaxID == F("btn_restart") ) device.restart();

    // Reset network settigns
    if( website.AjaxID == F("btn_rst_net") ){
        config.settings.networkSettings.SetDefaults();
        config.Save();
        return;
    }

    // Reset all settings
    if( website.AjaxID == F("btn_rst_all") ){
        config.SetDefaults();
        config.Save();
        return;
    }

}


// Create instance of page class and wrap methods for EmbAJAX
SystemPage systempage(
    []() { 
        systempage.ajax.handleRequest( 
            []() {
                systempage.HandleAjax();
            }
        ); 
    },
    []() { 
        systempage.InitializeAjax();
    }
);