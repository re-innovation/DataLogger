/*
 * LinkItONE.SetTimeFromGPS.Example.ino
 *
 * RTC set from GPS example on the LinkItONE platform.
 *
 * See README.md for full details
 *
 * Basic summary:
 *
 * - Every 5 seconds, sets RTC from GPS time
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

#include <LDateTime.h>
#include <LGPS.h>

/*
 * DataLogger Includes
 */

#include "DLUtility.Time.h"
#include "DLUtility.Location.h"
#include "DLTime.h"
#include "DLGPS.h"
#include "DLLocation.h"
#include "TaskAction.h"

static TM s_gpsTime;
static TM s_time;

static void printTime(TM * tm, bool endwithCRLF = false)
{
    if (tm->tm_hour < 10) { Serial.print("0"); }
    Serial.print(tm->tm_hour);
    Serial.print(":");

    if (tm->tm_min < 10) { Serial.print("0"); }
    Serial.print(tm->tm_min);
    Serial.print(":");

    if (tm->tm_sec < 10) { Serial.print("0"); }
    Serial.print(tm->tm_sec);

    if (endwithCRLF) { Serial.println(); }
}

static void printDate(TM * tm, bool endwithCRLF = false)
{
    if (tm->tm_mday < 10) { Serial.print("0"); }
    Serial.print(tm->tm_mday);
    Serial.print("-");

    if (tm->tm_mon < 10) { Serial.print("0"); }
    Serial.print(tm->tm_mon);
    Serial.print("-");

    uint8_t year = TWO_DIGIT_YEAR(tm->tm_year);
    if (year < 10) { Serial.print("0"); }
    Serial.print(year);

    if (endwithCRLF) { Serial.println(); }
}

void setup()
{
    // setup Serial port
    Serial.begin(115200);
    delay(10000);
    Location_Setup(0);
}

void loop()
{
    if (Time_GetTime(&s_gpsTime, TIME_GPS))
    {
        Serial.print("GPS time: ");
        printTime(&s_gpsTime, false);
        Serial.print(" ");
        printDate(&s_gpsTime, true);

        Serial.print("Setting RTC...");
        Time_SetPlatformTime(&s_gpsTime);

        Time_GetTime(&s_time, TIME_PLATFORM);
        Serial.print("RTC time: ");
        printTime(&s_time, false);
        Serial.print(" ");
        printDate(&s_time, true);
    }
    else
    {
        Serial.println("Waiting for valid GPS...");
    }
    delay(5000);
}
