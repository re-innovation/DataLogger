/*
 * DLTime.linkitone.cpp
 * 
 * Abstraction for Linkit ONE time resources
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

/*
 * Local Application Includes
 */

#include "DLUtility.Time.h"
#include "DLGPS.h"

/*
 * Local object and variables
 */
/* 
 * Time_GetTime
 *  Get time from platform.
 *  Returns TRUE if time pointer was altered.
 *  TRUE return does not imply accuracy of the returned time!
 * Args:
 *  tm: The time pointer to be filled
 *  type: Determines which time will be returned;
 */

bool Time_GetTime(TM * tm, TIME_TYPE type)
{
    if (!tm) { return false; }
    
    bool success = false;
    
    switch(type)
    {
    case TIME_PLATFORM:
        // TODO: Get platform RTC time
        break;
    case TIME_GPS:
        GPS_GetGPSTime(tm);
        success = true;
        break;
    case TIME_NETWORK:
        // TODO: Get time through network resources
        break;
    }
    
    return success;
}
