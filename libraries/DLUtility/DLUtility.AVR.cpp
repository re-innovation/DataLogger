/*
 * DLUtility.ArrayFunctions.cpp
 * 
 * A collection of generic utility routines for w
 *
 * Author: James Fowkes
 * Adapted from original software by Matt Little
 *
 * www.re-innovation.co.uk
 */

/* Arduino/AVR Includes */

#ifdef AVR_ARCH
#include <avr/pgmspace.h>  // Library for putting data into program memory
 
/*
 * Standard library includes
 */
#include <stdint.h>
#include "DLUtility.AVR.h"

/* getMaximum, getIndexOfMaximum
 * Template functions for getting the maximum of an array
 */

// Get a string from program memory
#define MAX_STRING 80      // Sets the maximum length of string probably could be lower
static char s_stringBuffer[MAX_STRING];  // A buffer to hold the string when pulled from program memory
char* getProgmemString(const char* str) {
	strcpy_P(s_stringBuffer, (char*)str);
	return s_stringBuffer;
}

#endif
