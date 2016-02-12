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
#include <string>

typedef std::string String;

#endif

#include <ctype.h>

/*
 * Application Includes
 */

#include "app.serial-interface.h"

static ON_SERIAL_REQUEST_CB s_handler;
static String s_req;

static bool s_debug_serial_requests = false;

/*
 * Private Methods
 */

static void handle_complete_request(String& req)
{
	int request_number = INVALID_REQUEST;
	bool valid_request = true;
	if (!s_handler) { return; }
	
	uint16_t len = req.length();
	valid_request &= (len != 0);
	valid_request &= (len <= 4);

	valid_request &= (req[0] == 's');
	valid_request &= (req[len-1] == '?');

	if (valid_request)
	{
		#ifdef ARDUINO
		if (s_debug_serial_requests)
		{
			Serial.print("Handling request ");
			Serial.print(req);
			Serial.print(" (");
			Serial.print(request_number);
			Serial.print(")");
		}
		#endif
		request_number = ((req[1] - '0') * 10) + (req[2] - '0');
		s_handler(request_number);
	}
	else
	{
		#ifdef ARDUINO
		Serial.print("Request ");
		Serial.print(req);
		Serial.print(" invalid.");
		#endif
		s_handler(INVALID_REQUEST);
	}
}

/*
 * Public Methods
 */

void APP_SerialInterface_Setup(ON_SERIAL_REQUEST_CB handler)
{
	s_handler = handler;
	s_req = "";
}

void APP_SerialInterface_HandleChar(char c)
{
	if (isalnum(c) || (c == '?'))
	{
		s_req += c;
	}

	if (c == '?')
	{
		handle_complete_request(s_req);
		s_req = "";
	}
}

void APP_SerialInterface_SetDebug(bool on)
{
	s_debug_serial_requests = on;
}
