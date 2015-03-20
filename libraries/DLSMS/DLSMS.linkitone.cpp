/*
 * DLSMS.linkitone.cpp
 *
 * James Fowkes
 * 20 March 2015
 *
 * www.re-innovation.co.uk
 */
 
#ifdef ARDUINO
#include <Arduino.h>
#else
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#endif

/*
 * Linkit ONE Includes
 */

#include <LGSM.h>

/*
 * Local Application Includes
 */

#include "DLSMS.h"
#include "DLSMS.linkitone.h"
 
/*
 * Public static class members
 */

LinkItOneSMSInterface::LinkItOneSMSInterface() {}
LinkItOneSMSInterface::~LinkItOneSMSInterface() {}

bool LinkItOneSMSInterface::send(char * pNumber, char * pMessage)
{
    
    if (!pNumber || !pMessage) { return false;}

    bool success = true;

    success &= LSMS.beginSMS(pNumber);
    success &= LSMS.ready();

    if (success)
    {
        LSMS.print(pMessage);
        success &= LSMS.endSMS();
    }

    return success;
}