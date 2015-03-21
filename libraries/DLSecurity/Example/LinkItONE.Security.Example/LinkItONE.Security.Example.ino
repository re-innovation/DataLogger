/*
 * LinkItONE.Security.Example.ino
 *
 * Security example for  the LinkItONE platform.
 *
 * See README.md for full details
 *
 * Basic summary:
 *
 * - Example of using GPS position for security.
 * - Randomly generates fake position to pass to security module.
 * - Prints alert message to serial port when unit goes >50m from "home"
 */

/*
 * Standard Library Includes
 */
#include <stdint.h>

/*
 * Arduino Library Includes
 */

#include <Wire.h>

/*
 * LinkIt One Includes
 */

#include <LGPS.h>

/*
 * DataLogger Includes
 */

#include "DLUtility.h"
#include "DLSettings.h"
#include "DLLocation.h"
#include "DLSecurity.h"
#include "TaskAction.h"

// Pointers to fuctionality objects
static LocationAlarm s_locationAlarm(onAlarmTripped, 50, NULL);
static LOCATION_2D s_currentLocation;

TaskAction gpsCheckTask(gpsCheckTaskFn, 10000, INFINITE_TICKS);
void gpsCheckTaskFn(void)
{
    float offset = (float)random(-100,100); // Random offset in meters

    Serial.print("Simulating location change of ");
    Serial.print(offset);
    Serial.println("m");

    // Convert offset to decimal degrees
    offset *= 360.0f;
    offset /= MEAN_EARTH_CIRCUMFERENCE_M;

    Location_UpdateNow();
    if ( Location_GetLocation_2D(&s_currentLocation) )
    {
        s_currentLocation.latitude += offset;
        s_locationAlarm.update(&s_currentLocation);
    }
}

void onAlarmTripped(void)
{
    Serial.println("Location alarm tripped!");
}

void setup()
{
    // setup Serial port
    Serial.begin(115200);

    delay(10000);

    Location_Setup(0);
    Location_UpdateNow();
    Location_GetLocation_2D(&s_currentLocation);
    s_locationAlarm.setHome(&s_currentLocation);

}

void loop()
{
    gpsCheckTask.tick();
}
