/*
 * LinkItONE.FileBlaster.Example.ino
 *
 * File write/read/destroy example for the LinkIt ONE
 *
 * See README.md for full details
 *
 * Basic summary:
 *
 * - Creates file with random filename in randomly named directory
 * - Blitzes with random data
 * - Reads and verifies data
 * - Deletes file
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

/*
 * DataLogger Includes
 */

#include "DLUtility.h"
#include "DLLocalStorage.h"

// Pointers to fuctionality objects
static LocalStorageInterface * s_sdCard;
static FILE_HANDLE s_fileHandle;

static char s_directoryName[] = "XXXXXXXX";

static void createRandomDirectory(void)
{
    uint8_t i = 0;
    for (i = 0; i < 8; ++i)
    {
        s_directoryName[i] = (char)random('A', 'Z'+1);
    }

    Serial.print("Creating directory ");
    Serial.println(s_directoryName);
}

void setup()
{
    // setup Serial port
    Serial.begin(115200);

    delay(10000);

    s_sdCard = LocalStorage_GetLocalStorageInterface(LINKITONE_SD_CARD);

}

void loop()
{
    createRandomDirectory();

    delay(1000);
}
