/**
 * @file        SystemPage.h
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


#ifndef SYSTEM_PAGE_H

    #define SYSTEM_PAGE_H

    // Global Libraries
    #include <Arduino.h>

    // Project Libraries
    #include "WebManager.h"
    #include "Logger.h"


    /** @class SystemPage
     *  @brief Server-side functions for system.html page */
    class SystemPage {

        public:

            PageHandler handler;                    // Handler for this page

            // Restart and reset elements

            EmbAJAXServerFunction btn_restart;      // Restart device button
            EmbAJAXServerFunction btn_rst_net;      // Reset network settings button
            EmbAJAXServerFunction btn_rst_all;      // Reset all settings button

            // Array of page elements
            EmbAJAXBase* page_elements[WEB_PAGE_COMMON_ELEMENTS_COUNT + 3
            ] = {
      
                WEB_PAGE_COMMON_ELEMENTS,       // Add the elements comment to every page

                &btn_restart,
                &btn_rst_net,
                &btn_rst_all

            };

            /** Construct a new page object
             * @param ajaxHander        Pointer to the lamda function that handles ajax for this page
             * @param initHandler       Pointer to the lamda function that initializes this page */
           SystemPage( void(*ajaxHandler)(), void(*initHandler)() ) : 
              
                btn_restart("btn_restart"),
                btn_rst_net("btn_rst_net"),
                btn_rst_all("btn_rst_all"),

                // Setup the EmbAJAX page base
                ajax(page_elements, "")
                {
                    handler.URL = "/system.html";
                    handler.ajaxHander = ajaxHandler;
                    handler.initHandler = initHandler;
                };

            EmbAJAXPage<sizeof(page_elements)/sizeof(EmbAJAXBase*)> ajax;       // Instance of EmbAJAX for this page

            /** Function to initialize AJAX on this page */
            void ICACHE_FLASH_ATTR InitializeAjax();

            /** Function to handle AJAX requests for this page */
            void ICACHE_FLASH_ATTR HandleAjax();


        protected:

    };
    

    extern SystemPage systempage;          // Global instance of this page

#endif              // SYSTEM_PAGE_H