/*
 * LinkItONE.SMSAndGPSAlarm.ino
 *
 * Security example for  the LinkItONE platform.
 *
 * See README.md for full details
 *
 * Basic summary:
 *
 * - Example of using GPS position for security.
 * - Sends alert SMS when unit goes >50m from "home"
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

#include <LGSM.h>
#include <LGPS.h>

/*
 * DataLogger Includes
 */

#include "DLUtility.h"
#include "DLSettings.h"
#include "DLLocation.h"
#include "DLGPS.h"
#include "DLSecurity.h"
#include "DLSMS.h"
#include "TaskAction.h"

/*
 *Defines and Typedefs
 */

#define ALARM_SMS_NUMBER "XXXXXXXXXXX" // Replace with actual number!
// Pointers to fuctionality objects
static SMSInterface * s_smsInterface;
static LocationAlarm s_locationAlarm(onAlarmTripped, 50, NULL);
static LOCATION_2D s_currentLocation;
static LOCATION_2D s_homeLocation;

TaskAction gpsCheckTask(gpsCheckTaskFn, 20000, INFINITE_TICKS);
void gpsCheckTaskFn(void)
{
    /* Update the GPS position and pass to alarm module */
    Location_UpdateNow();
    if ( Location_GetLocation_2D(&s_currentLocation) )
    {
        s_locationAlarm.update(&s_currentLocation);
    }
}

TaskAction smsUpdateTask(smsUpdateTaskFn, 60000, INFINITE_TICKS);
void smsUpdateTaskFn(void)
{
    /* Every minute, send distance from home via text */
    char distanceFromHomeBuffer[100];
    uint32_t diff = Location_diffInMeters(&s_currentLocation, &s_homeLocation);
    sprintf(distanceFromHomeBuffer, "Distance from home: %dm\0", diff);
    Serial.println(distanceFromHomeBuffer);
    s_smsInterface->send(ALARM_SMS_NUMBER, distanceFromHomeBuffer);
}

void onAlarmTripped(void)
{
    Serial.println("Location alarm tripped!");
    s_smsInterface->send(ALARM_SMS_NUMBER, "Location alarm tripped!");
}

void setup()
{
    // setup Serial port
    Serial.begin(115200);
    delay(10000);

    s_smsInterface = SMS_GetInterface(SMS_INTERFACE_LINKITONE);

    Location_Setup(0);

    // Wait for GPS to establish
    do
    {
      delay(1000);
      Location_UpdateNow();
      Serial.println("Searching for initial GPS position...");
    } while (!Location_GetLocation_2D(&s_homeLocation));
    
    Location_UpdateNow();
    Location_GetLocation_2D(&s_homeLocation);
    s_locationAlarm.setHome(&s_homeLocation);
}

void loop()
{
    gpsCheckTask.tick();
    smsUpdateTask.tick();
}
