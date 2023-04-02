/**
 * @file        Device.h
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
    #include "precomputils.h"

    #ifndef BUILD_NUMBER
        #include "Version.h"                // Defines local build number
    #endif


    // Used to stringify debug flags

    #define TEXTIFY(...) #__VA_ARGS__                   
    #define ESCAPEQUOTE(...) TEXTIFY(__VA_ARGS__)
    

    // In case build flags are missing for non-string flags

    #ifndef UPDATER_SKIP
        #define UPDATER_SKIP 1
    #endif

    #ifndef LOGGER_AS_SERIAL
        #define LOGGER_AS_SERIAL 1
    #endif

    #ifndef LOGGER_AS_SERVICE
        #define LOGGER_AS_SERVICE 0
    #endif

    #ifndef UPDATER_INTERVAL
        #define UPDATER_INTERVAL 300
    #endif   

    #ifndef LOGGER_TICK_INTERVAL
        #define LOGGER_TICK_INTERVAL 60
    #endif

    #ifndef LOGGER_TICKER
        #define LOGGER_TICKER 0
    #endif

    #ifndef LOGGER_LEVEL
        #define LOGGER_LEVEL 3
    #endif

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
    static const char flag_BUILD_TAG [] PROGMEM = ESCAPEQUOTE(BUILD_TAG);                       // Build tag - when used in Travis-CI comes from the GitHub Release
    static const char flag_BUILD_ENV [] PROGMEM = ESCAPEQUOTE(BUILD_ENV);                       // Build environmoent - Local or Travis-CI
    static const char flag_DEVICE_CODE [] PROGMEM = ESCAPEQUOTE(DEVICE_CODE);                   // Short code name for the device
    static const char flag_DEVICE_NAME [] PROGMEM = ESCAPEQUOTE(DEVICE_NAME);                   // Full device name
    static const uint flag_BUILD_NO = atoi(ESCAPEQUOTE(BUILD_NUMBER));                          // Get build number
    static const char flag_BUILD_TIMESTAMP [] PROGMEM = ESCAPEQUOTE(BUILD_TIMESTAMP);           // Set build date and time

#ifndef NETCHECK_DISABLE
    // Used by Netork Manager
    static const bool flag_NET_CHECKER = atoi(ESCAPEQUOTE(NET_CHECKER));                        // 0 - 1 to turn on internet connectivity checker
    static const char flag_NET_CHECK_SERVICE [] PROGMEM = ESCAPEQUOTE(NET_CHECK_SERVICE);       // Generate 204 script
    static const uint flag_NET_CHECK_INTERVAL = atoi(ESCAPEQUOTE(NET_CHECK_INTERVAL));          // Interval between checks for internet connectivity
#endif

#ifndef UPDATER_DISABLE
    // Used by Remote OTA library
    static const char flag_UPDATER_REPO [] PROGMEM = ESCAPEQUOTE(UPDATER_REPO);                 // GitHub reprositary holding this code
    static const char flag_UPDATER_USER [] PROGMEM = ESCAPEQUOTE(UPDATER_USER);                 // GitHub API user
    static const char flag_UPDATER_TOKEN [] PROGMEM = ESCAPEQUOTE(UPDATER_TOKEN);               // GitHub API OAUTH token
    static const char flag_UPDATER_SERVICE [] PROGMEM = ESCAPEQUOTE(UPDATER_SERVICE);           // Path to PHP used to return GitHub assets
    static const bool flag_UPDATER_SKIP = atoi(ESCAPEQUOTE(UPDATER_SKIP));                      // Skip any updates
    static const uint flag_UPDATER_INTERVAL = atoi(ESCAPEQUOTE(UPDATER_INTERVAL));              // Interval between update checks (sec)
#endif

#ifndef NO_LOGGING
    // Used by Logger library
    static const bool flag_LOGGER_AS_SERIAL = atoi(ESCAPEQUOTE(LOGGER_AS_SERIAL));              // 0 - 1 to turn on serial logging
#ifndef LOG_SERIAL_ONLY
    static const bool flag_LOGGER_AS_SERVICE = atoi(ESCAPEQUOTE(LOGGER_AS_SERVICE));            // 0 - 1 to turn on logging to Loggly service
    static const bool flag_LOGGER_TICKER = atoi(ESCAPEQUOTE(LOGGER_TICKER));                    // 0 - 1 to turn on ticking to Loggly service
    static const uint flag_LOGGER_TICK_INTERVAL = atoi(ESCAPEQUOTE(LOGGER_TICK_INTERVAL));      // Internal between ticks (sec)
    static const char flag_LOGGER_SERVICE_KEY [] PROGMEM = ESCAPEQUOTE(LOGGER_SERVICE_KEY);     // Loggly API key - stored in credentials.h for privacy
    static const char flag_LOGGER_SERVICE [] PROGMEM = ESCAPEQUOTE(LOGGER_SERVICE);             // Path to Loggly API
#endif
    static const uint flag_LOGGER_LEVEL = atoi(ESCAPEQUOTE(LOGGER_LEVEL));                      // 0 - 4 to set log level
    static const char flag_LOGGER_GLOBAL_TAGS [] PROGMEM = ESCAPEQUOTE(LOGGER_GLOBAL_TAGS);     // Tags to globally apply to logs
    static const uint flag_MONITOR_SPEED = atoi(ESCAPEQUOTE(MONITOR_SPEED));                    // Monitor baud
#endif

#ifndef TIMELOC_DISABLE
    // Used by Time / Location Library
    static const char flag_TLO_IPINFO_TOKEN [] PROGMEM = ESCAPEQUOTE(TLO_IPINFO_TOKEN);         // Token for IPInfo.io service
    static const char flag_TLO_IPINFO_SERVICE [] PROGMEM = ESCAPEQUOTE(TLO_IPINFO_SERVICE);     // URL for IPInfo.io service
#endif


    // Debouncer
    #define MAX_ISR         3       // Max number of interrupt service rountines supported (only used what's needed)
    #define DEBOUNCE_TIME   20      // milliseconds


    /** @class Hardware Switch Class
     *  @brief Sets up the hardware switch on an input pin with debounce 
     *         https://arduinoplusplus.wordpress.com/2021/02/05/interrupts-and-c-class-instances/ */
    class HardwareSwitch
    {
        public:

            // How is the pin used?
            enum PinType : uint8_t {
                ACTIVE_LOW,
                ACTIVE_HIGH
            };

            // State changes to the pin
            enum ActiveChange : uint8_t {
                INACTIVE,
                GOING_ACTIVE,
                GOING_INACTIVE
            };

            /** Constructor */
            ICACHE_FLASH_ATTR HardwareSwitch(uint8_t _pin, PinType _type, uint8_t _trigger ) : 
                _pin(_pin),
                _type(_type),
                _trigger(_trigger)
                {}
        
            /** Destructor */
            ICACHE_FLASH_ATTR ~HardwareSwitch()
            {
                detachInterrupt(digitalPinToInterrupt(_pin));
                _ISRUsed &= ~_BV(_myISRId);   // free up the ISR slot for someone else
            }

            /** Checks to see if there is a change in pin state
             *  @return Status change */
            ActiveChange ICACHE_FLASH_ATTR IsChanged(){ return _change; };

            /** Gets the pin state
             *  @return Pin state (true = active) */
            bool ICACHE_FLASH_ATTR GetState(){ return _state; };
        
            /** Starts up the pin debounce
             *  @return Successful if there was a spare ISR to use */
            bool ICACHE_FLASH_ATTR Begin();

            /** Gets the current counter for the pin
             *  @return Pin count */
            uint ICACHE_FLASH_ATTR GetCount(){ return _count; };

            /** Resets the pin counter to zero */
            void ICACHE_FLASH_ATTR ResetCount(){ _count = 0; };
        
            /** Resets the trigger for this pin */
            void ICACHE_FLASH_ATTR ResetTrigger(){ 
                _change = ActiveChange::INACTIVE;                     
                _triggerTime = 0;  
                _lastTriggerTime = 0;  
            };

    
        private:

            // Define the class variables
            uint8_t _pin;                 // The interrupt pin used
            PinType _type;               // If the active is when pin is high
            uint8_t _trigger;               // ISR trigger type

            uint8_t _myISRId;                // This is my instance ISR Id for _myInstance[x] and encoderISRx

            volatile uint16_t _count;        // Encoder interrupt counter
            volatile ActiveChange _change;                    
            volatile ulong _triggerTime;  
            volatile ulong _lastTriggerTime; 
            volatile bool _state;

            static uint8_t _ISRUsed;                     // Keep track of which ISRs are used (global bit field)
            static HardwareSwitch* _myInstance[];        // Callback instance for the ISR to reach instanceISR()
            
            void ICACHE_RAM_ATTR instanceISR();         // Instance ISR handler called from static ISR globalISRx
            
            // Declare all the [MAX_ISR] encoder ISRs
            #define GISRM1(i, _) static void ICACHE_RAM_ATTR CAT(globalISR,i)(); 
            EVAL(REPEAT( MAX_ISR, GISRM1, ~))
    };



    /** @class IOT Device Class
     *  @brief Expand the EspClass to add build flags, and contains any hardware specifics. */
    class Device : public EspClass
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

    extern Device device;        

#endif      // IOT_DEVICE_H
