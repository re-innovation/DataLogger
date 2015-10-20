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
 * DLHTTP.Header.cpp
 *
 * This class implements a key:value pair representing a single HTTP header.
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
// Header
//
//---------------------------------------------------------------------
Header::Header()
{
    m_name[0] = '\0';
    m_value[0] = '\0';
}

Header::~Header() {}
        
char * Header::getValue(void) { return m_value; }
char * Header::getName(void) { return m_name; }

bool Header::matchName(const char * name)
{
    return strcmp(m_name, name) == 0;
}

void Header::setName(const char * name)
{
    strncpy_safe(m_name, name, MAX_HTTP_HEADER_NAME_LENGTH);
}

void Header::setValue(const char * value)
{
    strncpy_safe(m_value, value, MAX_HTTP_HEADER_VALUE_LENGTH);
}
        
void Header::setFromLine(const char * p)
{
    uint8_t i = 0;
    
    if (!p) { return; }
    
    // Get the name (up to the colon character)
    while( *p && *p != ':' && i < MAX_HTTP_HEADER_NAME_LENGTH)
    {
        m_name[i++] = tolower( *p++ );
        m_name[i] = '\0';
    }
    
	// skip ':' and any spaces 
	if( *p )
		++p;
	p = skipSpaces(p);

	// Get value
    i = 0;
	while( *p && *p != '\0' && i < MAX_HTTP_HEADER_VALUE_LENGTH)
    {
		m_value[i++] = *p++;
        m_value[i] = '\0';
    }
}
