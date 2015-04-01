/*
 * DLUtility.Strings.cpp
 * 
 * Provides some helpful string functionality
 *
 * Author: James Fowkes
 *
 * www.re-innovation.co.uk
 */

/*
 * Arduino/C++ Library Includes
 */

#ifdef ARDUINO
#include <Arduino.h>
#else 
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#endif

/*
 * Local Application Includes
 */

#include "DLUtility.Strings.h"

/*
 * Private Functions 
 */

/*
 * Public Functions 
 */

/*
 * toLowerStr
 * Performs an in-place lowercase operation on pStr
 */
 
void toLowerStr(char * pStr)
{
    if (!pStr) { return; }
    
    while(*pStr)
    {
        *pStr = tolower(*pStr);
        pStr++;
    }
}

/*
 * skipSpaces
 *
 * Increments provided pointer until no longer a space char
 * as defined by isspace() library function, and returns
 * the new pointer
 */

char * skipSpaces(const char * line)
{
    if (!line) { return NULL; }
    
    while( *line && isspace(*line) ) { ++line; }
    
    return (char*)line;
}

/*
 * strncpy_safe
 *
 * Provides a wrapper around strncpy to avoid NULL terminator issues and the like
 * Also returns the number of characters copied
 */

uint32_t strncpy_safe(char * dst, char const * src, uint32_t max)
{
    if (!dst || !src) { return 0; }
    if (max == 0) { return 0; }

    if (max == 1) { dst[0] = '\0'; return 0; }

    max--; // Copy up to max-1 chars;
    
    uint32_t count = 0;
    bool stop = (*src == '\0') || (count == max);

    while (!stop)
    {
        *dst++ = *src++;
        count++;
        stop = (*src == '\0') || (count == max);
    }

    // Only NULL-terminate if max is greater than zero
    if (max > 0)
    {
        *dst = '\0';
    }

    return count;
}



/* FixedLengthAccumulator class */

/*
 * FixedLengthAccumulator::FixedLengthAccumulator
 *
 * Init the accumulator using a buffer and length of that buffer (INCLUDING space for terminating '\0')
 */

FixedLengthAccumulator::FixedLengthAccumulator(char * buffer, uint16_t length)
{
    if (buffer)
    {
        attach(buffer, length);
    }
    else
    {
        detach();
    }
}

FixedLengthAccumulator::~FixedLengthAccumulator() {}

/*
 * FixedLengthAccumulator::writeChar
 *
 * If there is space in the buffer, add the char to the string
 * Returns true if char was written, false if not
 */

bool FixedLengthAccumulator::writeChar(char c)
{
    if (m_buffer && m_writeIndex < m_maxLength)
    {
        m_buffer[m_writeIndex++] = c;
        m_buffer[m_writeIndex] = '\0';
        return true;
    }
    return false;
}

/*
 * FixedLengthAccumulator::writeString
 *
 * Copies chars from s until s is exhausted or the accumulator is full
 * Therefore, this function may only copy a partial length of s
 * Returns true if ALL of s was copied;
 */

bool FixedLengthAccumulator::writeString(const char * s)
{
    if (!s) { return false; }
    
    while(*s && (m_writeIndex < m_maxLength))
    {
        m_buffer[m_writeIndex++] = *s++;
        m_buffer[m_writeIndex] = '\0';
    }
    
    return (*s == '\0');
}

/*
 * FixedLengthAccumulator::writeLine
 *
 * As per writeString, but appends "\r\n" in addition to copying from s
 */

bool FixedLengthAccumulator::writeLine(const char * s)
{
    bool success = true;
    success &= writeString(s);
    success &= writeString("\r\n");
    return success;
}

/*
 * FixedLengthAccumulator::reset
 *
 * Makes the buffer appear to be an empty string
 */

void FixedLengthAccumulator::reset(void)
{
    m_writeIndex = 0;
    m_buffer[m_writeIndex] = '\0';
}

/*
 * FixedLengthAccumulator::c_str
 *
 * Returns pointer to the actual buffer
 */

char * FixedLengthAccumulator::c_str(void)
{
    return m_buffer;
}

/*
 * FixedLengthAccumulator::attach
 *
 * Redirects the accumulator to point at a new buffer
 */

void FixedLengthAccumulator::attach(char * buffer, uint16_t length)
{
    if (buffer)
    {
        m_buffer = buffer;
        m_maxLength = length-1;
        reset();
    }
}

/*
 * FixedLengthAccumulator::full
 *
 * Returns true if the accumulator is full
 */
 
bool FixedLengthAccumulator::isFull(void)
{
    return m_writeIndex == m_maxLength;
}

/*
 * FixedLengthAccumulator::detach
 *
 * Leaves the accumulator floating (and therefore safe)
 */

void FixedLengthAccumulator::detach(void)
{
    m_buffer = NULL;
    m_maxLength = 0;
    m_writeIndex = 0;
}

/*
 * FixedLengthAccumulator::length
 *
 * Returns the current length of the written buffer based on write index
 */

uint16_t FixedLengthAccumulator::length(void)
{
    return m_writeIndex;
}
