/**
 * @file        template.cpp
 * @author      Chris Gregg
 * 
 * @brief       Description of file
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


// Global Libraries
// #include <GlobalLibrary.h>

// Project Libraries
#include "template.h"
// #include "ProjectLibrary.h"


////////////////////////////////////////////
//// Example Class

// Public:

// Brief Description
ExampleClass::ExampleClass()
{

}


// Brief Description
bool ExampleClass::ExampleMethod( int paramexample )
{

}


// Protected:

// Brief Description
bool ExampleClass::ExampleMethod2( int paramexample )
{

}


// Create the global instance
#if !defined( NO_GLOBAL_INSTANCES ) && !defined( NO_GLOBAL_LIBRARY_NAME )
    ExampleClass exampleclass;
#endif
