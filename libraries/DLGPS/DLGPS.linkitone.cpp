/*
 * DLGPS.linkitone.cpp
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

#include "DLUtility.DateTime.h"
#include "DLUtility.Location.h"
#include "DLGPS.h"

/*
 * Local object and variables
 */
 
static gpsSentenceInfoStruct s_info;
static GPS_DATA s_parsedInfo;

static TaskAction updateTask(updateTaskFn, 0, INFINITE_TICKS);

/*
 * Public Functions 
 */

bool GPS_Setup(uint32_t updatePeriod)
{
    LGPS.powerOn();
    
    updateTask.SetInterval(updatePeriod * 1000UL);
    
    // No useful information comes back from powerOn, so must assume that it worked!
    return true; 
}

void GPS_Tick(void)
{
    updateTask.tick();
}

void GPS_UpdateNow(void)
{
    updateTaskFn();
}

void GPS_EnableUpdates(bool enable)
{
    updateTask.Enable(enable);
}

void GPS_SetUpdatePeriod(uint32_t newPeriod)
{
    updateTask.SetInterval(newPeriod * 1000UL);
}

bool GPS_GetLocation_2D(LOCATION_2D * pLocation)
{
    pLocation->latitude = s_parsedInfo.latitude;
    pLocation->longitude = s_parsedInfo.longitude;
}

bool GPS_GetLocation_3D(LOCATION_3D * pLocation)
{
    pLocation->latitude = s_parsedInfo.latitude;
    pLocation->longitude = s_parsedInfo.longitude;
    pLocation->altitude_m = s_parsedInfo.altitude;
}

uint16_t GPS_GetGPSTime(void)
{
    
}

/*
 * Private Function Definitions
 */

void updateTaskFn(void)
{
    LGPS.getData(&info);
    GPS_parseGPRMCSentence(&info.GPRMC, &s_parsedInfo);
}



