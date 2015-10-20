/*
 * DLTest.Mock.Time.cpp
 * 
 * Abstraction for PC time resources
 *
 * Author: James Fowkes
 *
 * www.re-innovation.co.uk
 */

/*
 * C++ Library Includes
 */

#include <stdint.h>
#include <time.h>

/*
 * Local Application Includes
 */

#include "DLUtility.Time.h"
#include "DLTime.h"

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

bool Time_GetTime(TM * ptm, TIME_TYPE type)
{
    if (!ptm) { return false; }
    
    bool success = false;

    time_t time_in_seconds = time (NULL);
    struct tm * dt = gmtime(&time_in_seconds);
    
    switch(type)
    {
    case TIME_PLATFORM:
        ptm->tm_sec = dt->tm_sec;
        ptm->tm_min = dt->tm_min;
        ptm->tm_hour = dt->tm_hour;
        ptm->tm_mday = dt->tm_mday;
        ptm->tm_mon = dt->tm_mon;
        ptm->tm_year = dt->tm_year;
        ptm->tm_wday = -1; // Don't calculate day in week
        ptm->tm_yday =  -1; // or day in year
        ptm->tm_isdst = false; // Assume that datetime is never with DST
        break;
    case TIME_GPS:
        break;
    case TIME_NETWORK:
        break;
    }
    
    return success;
}

void Time_SetPlatformTime(TM * tm)
{
    (void)tm;
}