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
#include "Env.h"
#include "NetworkManager.h"


////////////////////////////////////////////
//// Rode Settings Page Class

// Public:

// Function to initialize AJAX on this page
void ICACHE_FLASH_ATTR RodeSettingsPage::InitializeAjax(){

    LOG_HIGH( PSTR("(Page) Rode Settings - Initialize AJAX") );

    static char buffer[8];

    windlass_dia.setValue(itoa(316, buffer, 10));
    windlass_rpm.setValue(itoa(78, buffer, 10));
    windlass_rev.setChecked( true );
    chain_len.setValue(itoa(5000, buffer, 10));
    water_line.setValue(itoa(150, buffer, 10));
    rode_save.setEnabled( false ) ;

}


// Function to handle AJAX requests for this page
void ICACHE_FLASH_ATTR RodeSettingsPage::HandleAjax(){

    LOG_HIGH( PSTR("(Page) Rode Settings - Handle AJAX") );    

    // Reset Chain
    if( website.AjaxID == F("btn_reset") ) {
        DEBUG("Reset Chain");
    }

    // Save rode settings
    if( website.AjaxID == F("rode_save") ) {
        SaveRodeSettings();
        return;
    }
}


// Save the logger settings
void ICACHE_FLASH_ATTR RodeSettingsPage::SaveRodeSettings() {
    
        
    DEBUG(windlass_dia.value());
    DEBUG(windlass_rpm.value());
    DEBUG(windlass_rev.isChecked());
    DEBUG(chain_len.value());
    DEBUG(water_line.value());

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
