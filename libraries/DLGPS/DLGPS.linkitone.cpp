/*
 * DLLocation.linkitone.cpp
 * 
 * Abstraction for Linkit ONE GPS resources
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
 * Linkit ONE Includes
 */

#include <LGPS.h>

/*
 * Local Application Includes
 */

#include "DLUtility.Time.h"
#include "DLUtility.Location.h"
#include "DLLocation.h"
#include "DLGPS.h"

/*
 * Local object and variables
 */
 
static gpsSentenceInfoStruct s_rawInfo;
static GPS_DATA s_parsedInfo;
static bool s_infoIsValid = false;

/*
 * Public Functions 
 */

void GPS_UpdateNow(void)
{
    LGPS.getData(&s_rawInfo);
    s_infoIsValid = GPS_parseGPRMCSentence((char *)&s_rawInfo.GPRMC, &s_parsedInfo);
}

GPS_DATA const * GPS_GetInfo(void)
{
    return &s_parsedInfo;
}

bool GPS_InfoIsValid(void)
{
    return s_infoIsValid;
}