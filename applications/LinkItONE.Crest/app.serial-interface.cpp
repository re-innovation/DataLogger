/*
 * app.serial-interface.cpp
 *
 * James Fowkes
 *
 * 19 January 2016
 *
 * Handles serial request/response for CREST interface
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
 * Application Includes
 */

#include "app.serial-interface.h"

/*
 * Public Methods
 */

int APP_SerialInterface_HandleRequest(const char * req)
{
	if (!req) { return INVALID_REQUEST; }
	
	uint16_t len = strlen(req);
	if (len == 0) { return INVALID_REQUEST; }
	if (len > 4) { return INVALID_REQUEST; }

	if (req[0] != 's') { return INVALID_REQUEST; }
	if (req[len-1] != '?') { return INVALID_REQUEST; }

	return ((req[1] - '0') * 10) + (req[2] - '0');
}
