/*
 * LinkItONE.Settings.Reader.Example.ino
 *
 * Settings file reader example
 *
 * See README.md for full details
 *
 * Basic summary:
 *
 * - Reads settings from Datalogger.Settings on SD card storage and echoes to command line
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

#include <LSD.h>
#include <LStorage.h>

/*
 * DataLogger Includes
 */

#include "DLSettings.h"
#include "DLDataField.h"
#include "DLUtility.h"
#include "DLLocalStorage.h"

static char s_settingsFilename[] = "Datalogger.Settings";
static LocalStorageInterface * s_sdCard;
static char lineBuffer[100];

static void localPrintFn(char const * const toPrint)
{
    Serial.print(toPrint);
}

void setup()
{
    // setup Serial port
    Serial.begin(115200);

    delay(10000);

    s_sdCard = LocalStorage_GetLocalStorageInterface(LINKITONE_SD_CARD);

    Serial.println("*** Settings reader example ***");

    if (s_sdCard->fileExists(s_settingsFilename))
    {
        Serial.print("Reading settings from '");
        Serial.print(s_settingsFilename);
        Serial.println("'.");
    }
    else
    {
        Serial.print("Settings file '");
        Serial.print(s_settingsFilename);
        Serial.println("' not found.");
        return;
    }

    uint8_t hndl = s_sdCard->openFile(s_settingsFilename, false);
    while (!s_sdCard->endOfFile(hndl))
    {
        // Read from file into lineBuffer and strip CRLF endings
        s_sdCard->readLine(hndl, lineBuffer, 50, true);
        Serial.print("Reading setting line '");
        Serial.print(lineBuffer);
        Serial.println("'");
        Settings_readFromString(lineBuffer);
    }
    s_sdCard->closeFile(hndl);

    Settings_echoAllSet(localPrintFn);
}

void loop()
{
    
}
