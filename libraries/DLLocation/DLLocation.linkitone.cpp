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
#include "DLGPS.h"

/*
 * Private Functio Prototypes
 */

static void updateTaskFn(void);

/*
 * Local object and variables
 */
 
static TaskAction updateTask(updateTaskFn, 0, INFINITE_TICKS);

/*
 * Public Functions 
 */

bool Location_Setup(uint32_t updatePeriod)
{
    LGPS.powerOn();
    return true; 
    
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
    bool valid = GPS_InfoIsValid();
    if (valid)
    {
        GPS_DATA const * pData = GPS_GetInfo();
        pLocation->latitude = pData->latitude;
        pLocation->longitude = pData->longitude;
    }
    return valid;
}

bool Location_GetLocation_3D(LOCATION_3D * pLocation)
{
    bool valid = GPS_InfoIsValid();
    if (valid)
    {
        GPS_DATA const * pData = GPS_GetInfo();
        pLocation->latitude = pData->latitude;
        pLocation->longitude = pData->longitude;
        pLocation->altitude_m = pData->altitude;
    }
    return valid;
}

/*
 * Private Function Definitions
 */

static void updateTaskFn(void)
{
    GPS_UpdateNow();
}
