/**
 * @file        NetworkManager.h
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


#ifndef NETWORK_MANAGER_H

    #define NETWORK_MANAGER_H

    // Global Libraries
    #include <Arduino.h>

    // Project Libraries
    #include "WiFiManager.h"

 
    /** @class Network settings
     *  @brief A data structure class that contains the network settings. */
    class NetworkSettings {

        public:

            /** Resets logger settings to defaults */
            void ICACHE_FLASH_ATTR SetDefaults();

            WiFiSettings wifiSettings;                  // WiFi settings
        
            // Create a compare operators
            
            bool operator== ( const NetworkSettings& other ) const {
                return wifiSettings == other.wifiSettings;
            }
            bool operator!= ( const NetworkSettings& other ) const {
                return wifiSettings != other.wifiSettings;
            }

    };


    /** @class Network Manager
     *  @brief Manages all the network services */
    class NetworkManager {

        public:


            /** Start network services
             * @param settings      Reference to struct containing network settings */
            void ICACHE_FLASH_ATTR Begin( NetworkSettings& settings );

            /** Handles any repeated nertwork tasks */
            void Handle();

            // WiFi wrappers

            /** Reconnects the WiFi */
            void ICACHE_FLASH_ATTR ReconnectWifi() { _wifi.ReconnectWifi(); }

            /** Connects to a specific WiFi station
             *  @param id    ID of station, 0 to NET_MAX_STATIONS
             *  @return true:    Successfully connected
             *  @return false    Unsuccessfully connected */
            bool ICACHE_FLASH_ATTR ConnectWiFiStation( const int id = 0 ) { return _wifi.ConnectWiFiStation( id ); }

            /** Sets the WiFi mode
             *  @param mode     WiFiMode to set */
            void ICACHE_FLASH_ATTR SetWiFiMode( WiFiMode mode ) { _wifi.SetWiFiMode(mode); }

            /** Get the WiFi Client object
             * @return WiFiClient   Reference of reusable WiFiClient */
            WiFiClient& ICACHE_FLASH_ATTR GetWiFiClient() { return _wifi.GetWiFiClient(); }

            /** Gets the ID of the connected WiFi station
             *  @return uint    ID of the connected station */
            uint ICACHE_FLASH_ATTR GetConnectedStation() { return _wifi.GetConnectedStation(); }

            /** Check if a WiFi station is connected
             *  @param id       ID of station to check
             *  @return true:   The station is connected
             *  @return false:  The station is not connected */
            bool ICACHE_FLASH_ATTR IsStationConnected( uint id ) { return _wifi.IsStationConnected(id); }

            /** Get the IP address assigned by the connected WiFi station
             *  @return     Pointer to char array of IP address */
            char* ICACHE_FLASH_ATTR GetAssignedIP() { return _wifi.GetAssignedIP(); }


        protected:

            NetworkSettings* _settings;     // Pointer to data struct for the network settings

            // The only instances of the network services
            WiFiManager _wifi;              // WiFi service

    };


    extern NetworkManager network;        // Global instance of the network services

#endif              // NETWORK_MANAGER_H