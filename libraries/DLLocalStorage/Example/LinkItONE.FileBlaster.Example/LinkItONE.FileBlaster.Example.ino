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
 * LinkIt One Includes
 */

#include <LSD.h>

/*
 * DataLogger Includes
 */

#include "DLUtility.h"
#include "DLLocalStorage.h"


#define NUMBER_OF_FILES (10)

// Pointers to fuctionality objects
static LocalStorageInterface * s_sdCard;
static FILE_HANDLE s_fileHandle;

static char s_directoryName[] = "XXXXXXXX";
static char s_filePaths[NUMBER_OF_FILES][] = 
{
    "XXXXXXXX/XXXXXXXX",
    "XXXXXXXX/XXXXXXXX",
    "XXXXXXXX/XXXXXXXX",
    "XXXXXXXX/XXXXXXXX",
    "XXXXXXXX/XXXXXXXX",
    "XXXXXXXX/XXXXXXXX",
    "XXXXXXXX/XXXXXXXX",
    "XXXXXXXX/XXXXXXXX",
    "XXXXXXXX/XXXXXXXX",
    "XXXXXXXX/XXXXXXXX"
}

static void createRandomDirectory(void)
{
    uint8_t i = 0;
    for (i = 0; i < 8; ++i)
    {
        s_directoryName[i] = (char)random('A', 'Z'+1);
    }

    Serial.print("Creating directory ");
    Serial.println(s_directoryName);

    s_sdCard.mkDir(s_directoryName);
}

static void createAndDeleteRandomFiles(void)
{
    uint8_t i = 0;
    uint8_t j = 0;

    for (i = 0; i < NUMBER_OF_FILES; ++i)
    {
        for (j = 0; j < 8; ++j)
        {
            s_filePaths[i][j] = (char)random('A', 'Z'+1);
        }
        strncpy(s_filePaths, s_directoryName, 8);
        s_fileHandle = s_sdCard->openFile(s_filePaths[i], true);
        Serial.print("Creating file ");
        Serial.println(s_filePaths[i]);
    }

    Serial.println(s_directoryName);

    s_sdCard.mkDir(s_directoryName);
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
    createAndDeleteRandomFiles();
    delay(1000);
}
