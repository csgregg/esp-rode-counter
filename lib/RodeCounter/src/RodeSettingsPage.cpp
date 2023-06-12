/**
 * @file        RodeSettingsPage.cpp
 * @author      Chris Gregg
 * 
 * @brief       Server-side functions of rodesettings.html
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
#include "RodeSettingsPage.h"
#include "Logger.h"
#include "ConfigManager.h"


////////////////////////////////////////////
//// Rode Settings Page Class

// Public:

// Function to initialize AJAX on this page
void ICACHE_FLASH_ATTR RodeSettingsPage::InitializeAjax(){

    LOG_HIGH( PSTR("(Page) Rode Settings - Initialize AJAX") );

    RodeSettings settings = config.settings.rodeSettings;

    static char buffer[8];

    windlass_dia.setValue( itoa(settings.windlassDiameter, buffer, 10) );
    windlass_rpm.setValue( itoa(settings.windlassSpeed, buffer, 10) );
    windlass_rev.setChecked( settings.windlassReversed );
    chain_len.setValue(itoa( settings.chainLength/10, buffer, 10) );            // Convert mm to cm
    water_line.setValue(itoa( settings.waterLine/10, buffer, 10) );             // Convert mm to cm
    rode_save.setEnabled( false ) ;

}


// Function to handle AJAX requests for this page
void ICACHE_FLASH_ATTR RodeSettingsPage::HandleAjax(){

    LOG_HIGH( PSTR("(Page) Rode Settings - Handle AJAX") );    

    // Reset Chain
    if( website.AjaxID == F("btn_reset") ) {
        rodecounter.ResetRode();
    }

    // Save rode settings
    if( website.AjaxID == F("rode_save") ) {
        SaveRodeSettings();
    }
}


// Save the logger settings
void ICACHE_FLASH_ATTR RodeSettingsPage::SaveRodeSettings() {
    
    RodeSettings settings;
        
    settings.windlassDiameter = atoi( windlass_dia.value() );
    settings.windlassSpeed = atoi( windlass_rpm.value() );
    settings.windlassReversed = windlass_rev.isChecked();
    settings.chainLength = atoi( chain_len.value() ) * 10;      // Convert cm to mm
    settings.waterLine = atoi( water_line.value() ) * 10;       // Convert cm to mm

    config.settings.rodeSettings = settings;
    config.Save();

    rodecounter.LoadRodeSettings();

}



// Create instance of page class and wrap methods for EmbAJAX
RodeSettingsPage rodesettingspage(
    []() { 
        rodesettingspage.ajax.handleRequest( 
            []() {
                rodesettingspage.HandleAjax();
            }
        ); 
    },
    []() { 
        rodesettingspage.InitializeAjax();
    }
);
