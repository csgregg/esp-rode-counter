/**
 * @file        IndexPage.cpp
 * @author      Chris Gregg
 * 
 * @brief       Server-side functions of index.html
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
#include "IndexPage.h"
#include "Logger.h"
#include "Device.h"
#include "RodeCounter.h"


////////////////////////////////////////////
//// Index Page Class

// Public:

// Function to initialize AJAX on this page
void ICACHE_FLASH_ATTR IndexPage::InitializeAjax(){

    LOG_HIGH( PSTR("(Page) Index - Initialize AJAX") );

    UpdateWindlassStatus();

}


// Function to handle AJAX requests for this page
void ICACHE_FLASH_ATTR IndexPage::HandleAjax(){
    LOG_HIGH( PSTR("(Page) Index - Handle AJAX") );    

}


// Updates windlass status to page
void ICACHE_FLASH_ATTR IndexPage::UpdateWindlassStatus( ) {

    chain_up.setValue( rodecounter.GetChainDirection() == UP );
    chain_down.setValue( rodecounter.GetChainDirection() == DOWN );
    current_rode.setValue( rodecounter.GetCurrentRode()/10 );               // Convert mm to cm

    DEBUG(rodecounter.GetCurrentRode());
}


// Create instance of page class and wrap methods for EmbAJAX
IndexPage indexpage(
    []() { 
        indexpage.ajax.handleRequest( 
            []() {
                indexpage.HandleAjax();
            }
        ); 
    },
    []() { 
        indexpage.InitializeAjax();
    }
);
