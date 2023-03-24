/**
 * @file        IndexPage.h
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


#ifndef INDEX_PAGE_H

    #define INDEX_PAGE_H

    // Global Libraries
    #include <Arduino.h>

    // Project Libraries
    #include "WebManager.h"

    /** @class IndexPage
     *  @brief Server-side functions for index.html page */
    class IndexPage {

        public:

            PageHandler handler;                // Handler for this page

            EmbAJAXVarBool chain_up;            // Chain is moving up
            EmbAJAXVarBool chain_down;          // Chain is moving up
            EmbAJAXVarInt current_rode;         // Current rode deployed
            EmbAJAXVarInt water_line;           // Water line
            EmbAJAXVarInt rode_len;             // Overall length
            EmbAJAXVarInt warn_limit_1;         // 1st warning limit
            EmbAJAXVarInt warn_limit_2;         // 2nd warning limit
            EmbAJAXVarInt warn_limit_3;         // 2nd warning limit

            // Array of page elements
            EmbAJAXBase* page_elements[WEB_PAGE_COMMON_ELEMENTS_COUNT + 8] = {
      
                WEB_PAGE_COMMON_ELEMENTS,       // Add the elements comment to every page

                &chain_up,                      
                &chain_down,
                &current_rode,
                &water_line,
                &rode_len,
                &warn_limit_1,
                &warn_limit_2,
                &warn_limit_3,

            };

            /** Construct a new page object
             * @param ajaxHander        Pointer to the lamda function that handles ajax for this page
             * @param initHandler       Pointer to the lamda function that initializes this page */
            IndexPage( void(*ajaxHandler)(), void(*initHandler)() ) : 

                chain_up("chain_up"),
                chain_down("chain_down"),
                current_rode("current_rode"),
                water_line("water_line"),
                rode_len("rode_len"),
                warn_limit_1("warn_limit_1"),
                warn_limit_2("warn_limit_2"),
                warn_limit_3("warn_limit_3"),

                ajax( page_elements, "" )
                {
                    handler.URL = "/index.html";
                    handler.ajaxHander = ajaxHandler;
                    handler.initHandler = initHandler;
                };

            EmbAJAXPage<sizeof(page_elements)/sizeof(EmbAJAXBase*)> ajax;       // Instance of EmbAJAX for this page

            /** Function to initialize AJAX on this page */
            void ICACHE_FLASH_ATTR InitializeAjax();

            /** Function to handle AJAX requests for this page */
            void ICACHE_FLASH_ATTR HandleAjax();

            /** Updates windlass status to page */
            void ICACHE_FLASH_ATTR UpdateWindlassStatus();
    };
    
    
    extern IndexPage indexpage;     // Global instance of this page

#endif          // INDEX_PAGE_H