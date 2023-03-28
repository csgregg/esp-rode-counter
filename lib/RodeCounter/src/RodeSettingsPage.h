/**
 * @file        RodeSettingsPage.h
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


#ifndef RODESETTINGS_PAGE_H

    #define RODESETTINGS_PAGE_H

    // Global Libraries
    #include <Arduino.h>

    // Project Libraries
    #include "WebManager.h"

    /** @class RodeSettingsPage
     *  @brief Server-side functions for rodesettings.html page */
    class RodeSettingsPage {

        public:

            PageHandler handler;                // Handler for this page

            EmbAJAXTextInput<4> windlass_dia;   // Input box for windlass diameter
            EmbAJAXTextInput<4> windlass_rpm;   // Input box for windlass speed
            EmbAJAXCheckButton windlass_rev;    // Check box for windlass reverse
            EmbAJAXTextInput<5> chain_len;      // Input box for overall chain length
            EmbAJAXTextInput<5> water_line;     // Input box for chain water line 
            EmbAJAXServerFunction btn_reset;    // Reset chain button
            EmbAJAXServerFunction rode_save;    // Save settings button

            // Array of page elements
            EmbAJAXBase* page_elements[WEB_PAGE_COMMON_ELEMENTS_COUNT + 7] = {
      
                WEB_PAGE_COMMON_ELEMENTS,       // Add the elements comment to every page

                &windlass_dia,
                &windlass_rpm,
                &windlass_rev,
                &chain_len,
                &water_line,
                &btn_reset,
                &rode_save,

            };

            /** Construct a new page object
             * @param ajaxHander        Pointer to the lamda function that handles ajax for this page
             * @param initHandler       Pointer to the lamda function that initializes this page */
            RodeSettingsPage( void(*ajaxHandler)(), void(*initHandler)() ) : 

                windlass_dia("windlass_dia"),
                windlass_rpm("windlass_rpm"),
                windlass_rev("windlass_rev",""),
                chain_len("chain_len"),
                water_line("water_line"),
                btn_reset("btn_reset"),
                rode_save("rode_save"),

                ajax( page_elements, "" )
                {
                    handler.URL = "/rodesettings.html";
                    handler.ajaxHander = ajaxHandler;
                    handler.initHandler = initHandler;
                };

            EmbAJAXPage<sizeof(page_elements)/sizeof(EmbAJAXBase*)> ajax;       // Instance of EmbAJAX for this page

            /** Function to initialize AJAX on this page */
            void ICACHE_FLASH_ATTR InitializeAjax();

            /** Function to handle AJAX requests for this page */
            void ICACHE_FLASH_ATTR HandleAjax();

            /** Save the logger settings */
            void ICACHE_FLASH_ATTR SaveRodeSettings();
    };
    
    
    extern RodeSettingsPage rodesettingspage;     // Global instance of this page

#endif          // RODESETTINGS_PAGE_H