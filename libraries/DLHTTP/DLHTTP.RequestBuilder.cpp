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
    
    m_headerCount = 0;
    
    accumulator.detach();
    accumulator.attach(buf, maxLength); 

    m_method = NULL;
    m_url = NULL;
    m_body = NULL;
}

RequestBuilder::~RequestBuilder() {}

void RequestBuilder::setMethodAndURL(const char* method, const char* url)
{
    m_method = method;
    m_url = url;
}

void RequestBuilder::putHeader( const char* header, const char* value )
{
    if (m_headerCount == MAX_HTTP_HEADERS) { return; }
    m_headers[m_headerCount].setName(header);
    m_headers[m_headerCount++].setValue(value);
}

void RequestBuilder::putBody(const char * body)
{
    m_body = body;
}

void RequestBuilder::writeToBuffer(bool addContentLengthHeader)
{

    if (!m_method || !m_url) { return; }
    
    /* Write status line */
    accumulator.reset();
    accumulator.writeString(m_method);
    accumulator.writeChar(' ');
    accumulator.writeString(m_url);
    accumulator.writeChar(' ');
    accumulator.writeString("HTTP/1.1");
    accumulator.writeString(CRLF);
    
    /* Write header lines */
    uint8_t i = 0;

    for (i = 0; i < m_headerCount; i++)
    {
        accumulator.writeString(m_headers[i].getName());
        accumulator.writeString(": ");
        accumulator.writeString(m_headers[i].getValue());
        accumulator.writeString(CRLF);
    }
    
    if (addContentLengthHeader && m_body)
    {
        char lengthStr[5];
        sprintf(lengthStr, "%d", (int)strlen(m_body));
        
        accumulator.writeString("Content-Length: ");
        accumulator.writeString(lengthStr);
        accumulator.writeString(CRLF);
    }
    
    /* Write body */ 
    if (m_body)
    {
        accumulator.writeString(CRLF);
        accumulator.writeString(m_body);
        accumulator.writeString(CRLF);
    }
    
    /* Request finishes with blank line */

    accumulator.writeString(CRLF);
}
