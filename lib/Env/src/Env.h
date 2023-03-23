/**
 * @file        Env.h
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


#ifndef IOT_DEVICE_H

    #define IOT_DEVICE_H

    // Global Libraries
    #include <Arduino.h>
    
    // Project Libraries
    #include "Credentials.h"                // Contains private definitions (excluded from repo)

    #ifndef BUILD_NUMBER
        #include "Version.h"                // Defines local build number
    #endif


    // Used to stringify debug flags

    #define TEXTIFY(...) #__VA_ARGS__                   
    #define ESCAPEQUOTE(...) TEXTIFY(__VA_ARGS__)
    

    // In case build flags are missing for non-string flags

    #ifndef MONITOR_SPEED
        #define MONITOR_SPEED 115200
    #endif

    #ifndef BUILD_NUMBER
        #define BUILD_NUMBER 0
    #endif

    #ifndef BUILD_ENV
        #define BUILD_ENV None
    #endif

    #ifndef BUILD_TIMESTAMP
        #define BUILD_TIMESTAMP None
    #endif


    // Sizes
    #define FLAG_MAX_BUILD_NO_LEN (5+1)                 // Max length of the build no flag
    #define FLAG_MAX_BUILD_TIMESTAMP_LEN (24+1)         // Max length of the build timestamp flag
    #define FLAG_MAX_CHIPIN_LEN (8+1)                   // Max length of the chip ID
    #define FLAG_MAX_BUILDENV_LEN (32+1)                // Max length of the build environment flag

   // Detect double reset
    #define DRD_TIMEOUT 3           // Number of seconds after reset during which a subseqent reset will be considered a double reset.
    #define DRD_ADDRESS 0           // RTC Memory Address for the DoubleResetDetector to use


    // Physical board
    static const char flag_BOARD [] PROGMEM = ESCAPEQUOTE(BOARD);                               // Specific board

    // General build details
    static const char flag_BUILD_TAG [] PROGMEM = ESCAPEQUOTE(BUILD_TAG);                       // Build tag
    static const char flag_BUILD_ENV [] PROGMEM = ESCAPEQUOTE(BUILD_ENV);                       // Build environmoent - Local
    static const char flag_DEVICE_CODE [] PROGMEM = ESCAPEQUOTE(DEVICE_CODE);                   // Short code name for the device
    static const char flag_DEVICE_NAME [] PROGMEM = ESCAPEQUOTE(DEVICE_NAME);                   // Full device name
    static const uint flag_BUILD_NO = atoi(ESCAPEQUOTE(BUILD_NUMBER));                          // Get build number
    static const char flag_BUILD_TIMESTAMP [] PROGMEM = ESCAPEQUOTE(BUILD_TIMESTAMP);           // Set build date and time
    
    static const uint flag_MONITOR_SPEED = atoi(ESCAPEQUOTE(MONITOR_SPEED));                    // Monitor baud


    /** @class IOT Device Class
     *  @brief Expand the EspClass to add build flags, and contains any hardware specifics. */
    class Env : public EspClass
    {

        public:

            enum StartMode {
                NORMAL,
                DOUBLERESET,
            };

            /** Sets up the device hardware and build environment */
            void ICACHE_FLASH_ATTR Begin();

            /** Get the build number
             *  @returns String containing the number of the build */
            char* ICACHE_FLASH_ATTR GetBuildNo() { return _buildNo; };

            /** Get the build time stamp
             *  @returns String containing the date and time of the build */
            char* ICACHE_FLASH_ATTR GetBuildTime() { return _buildTime; };

            /** Get the chip ID
             *  @returns String containing the chip ID */            
            char* ICACHE_FLASH_ATTR GetChipId() { return _chipID; };

            /** Get the build environment
             *  @returns String containing the build environment */     
            char* ICACHE_FLASH_ATTR GetBuildEnv() { return _buildEnv; };

            /** Get the mode the device was started in
             *  @returns StartMode */         
            StartMode ICACHE_FLASH_ATTR GetStartMode() { return _startMode; };

            /** Handles any repeating device actions */    
            void ICACHE_FLASH_ATTR Handle();


        protected:

            char _buildNo[FLAG_MAX_BUILD_NO_LEN];               // Stores the build number as char array                // TODO - this is double storage. Must be a better way.
            char _buildTime[FLAG_MAX_BUILD_TIMESTAMP_LEN];      // Stores the build time stamp as char array
            char _chipID[FLAG_MAX_CHIPIN_LEN];                  // Stores the chip ID as char array
            char _buildEnv[FLAG_MAX_BUILDENV_LEN];              // Stores build environment as char array

            StartMode _startMode = NORMAL;                      // The mode the device was started in. Defaults to NORMAL
    };

    // Declaring the global instances

    extern Env device;        

#endif      // IOT_DEVICE_H
