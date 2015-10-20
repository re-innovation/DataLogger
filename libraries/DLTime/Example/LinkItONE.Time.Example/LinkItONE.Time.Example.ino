/*
 * LinkItONE.Time.Example.ino
 *
 * RTC Get/Set example on the LinkItONE platform.
 *
 * See README.md for full details
 *
 * Basic summary:
 *
 * - Reads from RTC, echoes to serial, randomly adds up to 60 minutes, writes back to RTC and repeats
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

// Pointers to fuctionality objects
static TM s_time;

static void printTime(bool endwithCRLF = false)
{
    if (s_time.tm_hour < 10) { Serial.print("0"); }
    Serial.print(s_time.tm_hour);
    Serial.print(":");

    if (s_time.tm_min < 10) { Serial.print("0"); }
    Serial.print(s_time.tm_min);
    Serial.print(":");

    if (s_time.tm_sec < 10) { Serial.print("0"); }
    Serial.print(s_time.tm_sec);

    if (endwithCRLF) { Serial.println(); }
}

static void printDate(bool endwithCRLF = false)
{
    if (s_time.tm_mday < 10) { Serial.print("0"); }
    Serial.print(s_time.tm_mday);
    Serial.print(":");

    if (s_time.tm_mon < 10) { Serial.print("0"); }
    Serial.print(s_time.tm_mon);
    Serial.print(":");

    uint8_t year = TWO_DIGIT_YEAR(s_time.tm_year);
    if (year < 10) { Serial.print("0"); }
    Serial.print(year);

    if (endwithCRLF) { Serial.println(); }
}

void setup()
{
    // setup Serial port
    Serial.begin(115200);
    delay(10000);

    Time_GetTime(&s_time, TIME_PLATFORM);

    Serial.print("Example started at ");
    printTime();
    Serial.print(" on ");
    printDate(true);
}

void loop()
{
    Time_GetTime(&s_time, TIME_PLATFORM);

    Serial.print("Got time ");
    printTime(true);    

    Serial.print("Adding ");
    uint16_t randomSeconds = random(0, 3600);
    Serial.print(randomSeconds);
    Serial.print(" seconds.");

    while(randomSeconds--)
    {
        time_increment_seconds(&s_time);
    }

    Time_SetPlatformTime(&s_time);

    delay(10000);
}
