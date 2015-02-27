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
#include <TaskAction.h>

/*
 * Local Application Includes
 */

#include "DLUtility.Time.h"
#include "DLUtility.Location.h"
#include "DLLocation.h"
#include "DLLocation.GPS.h"

/*
 * Private Functio Prototypes
 */

static void updateTaskFn(void);

/*
 * Local object and variables
 */
 
static gpsSentenceInfoStruct s_rawInfo;
static GPS_DATA s_parsedInfo;

static TaskAction updateTask(updateTaskFn, 0, INFINITE_TICKS);

/*
 * Public Functions 
 */

bool Location_Setup(uint32_t updatePeriod)
{
    LGPS.powerOn();
    
    updateTask.SetInterval(updatePeriod * 1000UL);
    
    // No useful information comes back from powerOn, so must assume that it worked!
    return true; 
}

void Location_Tick(void)
{
    updateTask.tick();
}

void Location_UpdateNow(void)
{
    updateTaskFn();
}

void Location_EnableUpdates(bool enable)
{
    updateTask.Enable(enable);
}

void Location_SetUpdatePeriod(uint32_t newPeriod)
{
    updateTask.SetInterval(newPeriod * 1000UL);
}

bool Location_GetLocation_2D(LOCATION_2D * pLocation)
{
    pLocation->latitude = s_parsedInfo.latitude;
    pLocation->longitude = s_parsedInfo.longitude;
}

bool Location_GetLocation_3D(LOCATION_3D * pLocation)
{
    pLocation->latitude = s_parsedInfo.latitude;
    pLocation->longitude = s_parsedInfo.longitude;
    pLocation->altitude_m = s_parsedInfo.altitude;
}

void Location_GetGPSTime(TM * time)
{
    if (!time) { return; }
    
    time->tm_sec = s_parsedInfo.sec;
    time->tm_min = s_parsedInfo.min;
    time->tm_hour = s_parsedInfo.hour;
    time->tm_mday = s_parsedInfo.dd;
    time->tm_mon = s_parsedInfo.mm;
    time->tm_year = s_parsedInfo.yy;
}

/*
 * Private Function Definitions
 */

static void updateTaskFn(void)
{
    LGPS.getData(&s_rawInfo);
    GPS_parseGPRMCSentence((char *)&s_rawInfo.GPRMC, &s_parsedInfo);
}



