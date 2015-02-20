/*
 * linkitonewifi.cpp
 * 
 * Connect and communicate over Wifi
 *
 * Author: James Fowkes
 *
 * www.re-innovation.co.uk
 */

/*
 * Arduino/C++ Library Includes
 */

#include <Arduino.h>

/*
 * LinkIt One Includes
 */

#include <LGPRS.h>
#include <LGPRSClient.h>

#include "DLNetwork.h"
#include "DLNetwork.linkitone.h"

/*
 * Private Variables
 */

/*
 * Public Functions 
 */

LinkItOneWiFi::LinkItOneWiFi() {}

LinkItOneWiFi::~LinkItOneWiFi() {}

bool LinkItOneWiFi::tryConnection(uint8_t timeoutSeconds)
{
	// WIFI FUNCTIONALITY NOT YET IMPLEMENTED
    (void)timeoutSeconds;
    return false;
}

bool LinkItOneWiFi::HTTPGet(char const * const url, char * request, char * response, bool useHTTPS)
{
	// WIFI FUNCTIONALITY NOT YET IMPLEMENTED
	(void)url;
	(void)request;
	(void)response;
	(void)useHTTPS;
	return false;
}

bool LinkItOneWiFi::isConnected(void) { return false; }
