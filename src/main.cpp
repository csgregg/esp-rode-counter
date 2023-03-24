/* ESP Based Rode Counter

MIT License

Copyright (c) 2023 Chris Gregg

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
SOFTWARE.

-----------------------------------------------------------------------------

Setup and Loop

*/


#include "Env.h"
#include "ConfigManager.h"
#include "Logger.h"
#include "NetworkManager.h"
#include "WebManager.h"


void ICACHE_FLASH_ATTR setup() {

#ifndef NO_LOGGING
    delay( 1000 );
    Serial.begin(flag_MONITOR_SPEED);
#endif

    // Services started in the proper order

    device.Begin();
    config.Begin( true );
    network.Begin( config.settings.networkSettings );
    website.Begin( flag_DEVICE_NAME );

    LOG(PSTR("(Loop) Starting"));
    
}


void loop() {

    // Handle each service set
    device.Handle();
    network.Handle();
    website.Handle();

}