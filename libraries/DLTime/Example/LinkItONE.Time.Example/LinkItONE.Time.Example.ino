/*
 * LinkItONE.Time.Example.ino
 *
 * RTC Get/Set example on the LinkItONE platform.
 *
 * See README.md for full details
 *
 * Basic summary:
 *
 * - Reads from RTC, echoes to serial, randomly adds up to +/- 30 minutes and repeats
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

/*
 * DataLogger Includes
 */

#include "DLTime.h"

// Pointers to fuctionality objects

void setup()
{
    // setup Serial port
    Serial.begin(115200);

    delay(10000);
}

void loop()
{
    
}
