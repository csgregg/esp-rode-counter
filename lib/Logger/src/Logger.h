/**
 * @file        Logger.h
 * @author      Chris Gregg
 * @brief       Packages debug and logging to the serial port and Loggly.com service for
 *              simple logging or debug of code. Macros are defined to simplifiy common usage. 
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


#ifndef LOGGER_H

    #define LOGGER_H

    // #define NO_LOGGING               // Turn off all debug if necessary

    // Macros to simplifiy common usage

    #ifndef NO_LOGGING

        #define LOG( text )   {Serial.printf( "DEBUG: %s %s Line: %i - ", __FILE__, __FUNCTION__, __LINE__ ); Serial.println( text );}
        #define LOGF( ... )   {Serial.printf( "DEBUG: %s %s Line: %i - ", __FILE__, __FUNCTION__, __LINE__ ); Serial.printf( __VA_ARGS__ );}
        #define DEBUG_WAIT      {Serial.printf( "DEBUG: (Wait) %s %s Line: %i\n", __FILE__, __FUNCTION__, __LINE__ ); while( Serial.read() != ' ' ) { yield(); };}
        #define DEBUG_LINE      {Serial.printf( "DEBUG: (Line) %s %s Line: %i\n", __FILE__, __FUNCTION__, __LINE__ );}
        #define DEBUG_STOP      {Serial.printf( "\n\nDEBUG (Stop) %s %s Line: %i\n", __FILE__, __FUNCTION__, __LINE__ ); while( true ) { yield(); };}

    #else

        // Empty macros which do nothing when logging fully turned off in build

        #define LOG( text )
        #define LOGF( ... )
        #define DEBUG_WAIT
        #define DEBUG_LINE
        #define DEBUG_STOP

    #endif

#endif              // LOGGER_H

