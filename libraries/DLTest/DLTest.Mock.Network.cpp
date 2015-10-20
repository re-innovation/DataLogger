/*
 * DLTest.Mock.Network.cpp
 * 
 * Mock object for network access
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

#include "DLNetwork.h"
#include "DLTest.Mock.Network.h"

/*
 * Private Variables
 */

 /*
 * Public Functions 
 */

NetworkInterface * Network_GetNetwork(NETWORK_INTERFACE interface)
{
    (void)interface;
    return new TestNetworkInterface();
}

TestNetworkInterface::TestNetworkInterface()
{
}

bool TestNetworkInterface::tryConnection(uint8_t timeoutSeconds)
{
    (void)timeoutSeconds;
    return true;
}

bool TestNetworkInterface::sendHTTPRequest(const char * const url, const char * request, char * response, bool useHTTPS)
{
    (void)useHTTPS;
    (void)request;
    (void)response;

    bool success = true; // TODO: connect to server

    if (success)
    {
        // TODO: send request and read response
    }
    else
    {
        Serial.print("sendHTTPRequest: Failed to connect to ");
        Serial.println(url);
    }
    return success;
}

bool TestNetworkInterface::isConnected(void) { return true; }
