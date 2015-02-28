/*
 * HappyHTTP - a simple HTTP library
 * Version 0.1
 * 
 * Copyright (c) 2006 Ben Campbell
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software in a
 * product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not
 * be misrepresented as being the original software.
 * 
 * 3. This notice may not be removed or altered from any source distribution.
 *
 */

/* 
 * DLHTTP.RequestBuilder.cpp
 *
 * This class handles creation of HTTP requests.
 * It is a heavily stripped-down and altered version of HappyHTTP by Ben Campbell (see above).
 */

/*
 * C++ Library Includes
 */

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#endif
/*
 * Local Application Includes
 */

#include "DLUtility.Strings.h"
#include "DLHTTP.h"

//---------------------------------------------------------------------
//
// RequestBuilder
//
//---------------------------------------------------------------------

RequestBuilder::RequestBuilder(char * buf, uint16_t maxLength):
    accumulator(NULL, 0)
{
    if (!buf) { return; }
    
    accumulator.detach();
    accumulator.attach(buf, maxLength);    
}

RequestBuilder::~RequestBuilder() {}

void RequestBuilder::setMethodAndURL(const char* method, const char* url)
{
    accumulator.writeString(method);
    accumulator.writeChar(' ');
    accumulator.writeString(url);
    accumulator.writeChar(' ');
    accumulator.writeString("HTTP/1.1");
    accumulator.writeString(CRLF);   
}

void RequestBuilder::putHeaders( const char* header, const char* value )
{
	accumulator.writeString(header);
    accumulator.writeString(": ");
    accumulator.writeString(value);
    accumulator.writeString(CRLF);
}

void RequestBuilder::putBody(const char * body)
{
    accumulator.writeString(CRLF);
    accumulator.writeString(body);
    accumulator.writeString(CRLF);
}
