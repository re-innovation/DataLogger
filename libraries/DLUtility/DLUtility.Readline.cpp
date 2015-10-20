/*
 * DLUtility.Readline.cpp
 * 
 * Provides functionality to read a single line from a file or other stream source into a buffer
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
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#endif

/*
 * Local Application Includes
 */

#include "DLUtility.h"

/*
 * Public Functions 
 */

uint32_t readLineWithReadFunction(READFN fn, char * buffer, uint32_t n, bool stripCRLF)
{
	uint32_t readCount = 0;
	char next;
	bool eol = false;

	do
	{
		next = fn();

		switch(next)
		{
		case '\n':
			// End of line, but add line feed to the buffer
			eol = true;
			buffer[readCount++] = next;
			break;
		case '\0':
			// End of line, DO NOT add terminator to the buffer
			eol = true;
			break;
		default:
			buffer[readCount++] = next;
			break;
		}
	} while ((readCount < n) && !eol);

	if (stripCRLF)
	{
		readCount--;
		while((buffer[readCount] == '\r') || (buffer[readCount] == '\n'))
		{
			buffer[readCount--] = '\0';
		}
		readCount++; // Put read pointer back to position of last NULL
	}

	if (readCount < n)
	{
		buffer[readCount] = '\0'; // NULL-terminate if there is room left in the buffer
	}

	return readCount;
}