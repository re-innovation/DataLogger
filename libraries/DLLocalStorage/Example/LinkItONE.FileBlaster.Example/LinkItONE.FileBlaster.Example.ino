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
#include <string.h>

/*
 * LinkIt One Includes
 */

#ifdef ARDUINO
#include <LSD.h>
#else
#include "DLTest.Mock.Serial.h"
#include "DLTest.Mock.delay.h"
#include "DLTest.Mock.random.h"
#endif

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
static char s_filePaths[NUMBER_OF_FILES][20] =
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
};

static void createRandomDirectory(void)
{
    uint8_t i = 0;
    for (i = 0; i < 8; ++i)
    {
        s_directoryName[i] = (char)random('A', 'Z'+1);
    }

    Serial.print("Creating directory ");
    Serial.println(s_directoryName);

    s_sdCard->mkDir(s_directoryName);
}

static void createRandomFiles(void)
{
    uint8_t i = 0;
    uint8_t j = 0;

    for (i = 0; i < NUMBER_OF_FILES; ++i)
    {

        strncpy(s_filePaths[i], s_directoryName, 8);

        for (j = 0; j < 8; ++j)
        {
            s_filePaths[i][j+9] = (char)random('A', 'Z'+1);
        }

        Serial.print("Creating file ");
        Serial.print(s_filePaths[i]);

        s_fileHandle = s_sdCard->openFile(s_filePaths[i], true);

        Serial.print(" handle ");
        Serial.println(s_fileHandle);

        s_sdCard->write(s_fileHandle, "ABC");
        s_sdCard->closeFile(s_fileHandle);

    }
}

static void readFromFiles(void)
{
    uint8_t i = 0;
    char buffer[10];
    for (i = 0; i < NUMBER_OF_FILES; ++i)
    {
        s_fileHandle = s_sdCard->openFile(s_filePaths[i], false);
        s_sdCard->readLine(s_fileHandle, buffer, 10, true);
        Serial.print("Read '");
        Serial.print(buffer);
        Serial.print("' from ");
        Serial.println(s_filePaths[i]);
        s_sdCard->closeFile(s_fileHandle);
    }
}

static void checkFilesExist(void)
{
    uint8_t i = 0;
    for (i = 0; i < NUMBER_OF_FILES; ++i)
    {
        Serial.print("Testing file ");
        Serial.print(s_filePaths[i]);
        Serial.print(" exists... ");
        if (s_sdCard->fileExists(s_filePaths[i]))
        {
            Serial.println("success!");
        }
        else
        {
            Serial.println("fail!");
        }
    }
}

static void deleteFiles(void)
{
    uint8_t i = 0;
    for (i = 0; i < NUMBER_OF_FILES; ++i)
    {
        Serial.print("Deleting file ");
        Serial.println(s_filePaths[i]);
        s_sdCard->removeFile(s_filePaths[i]);
    }
}

void setup()
{
    // setup Serial port
    Serial.begin(115200);

    delay(10000);

    s_sdCard = LocalStorage_GetLocalStorageInterface(LINKITONE_SD_CARD);

    createRandomDirectory();
    createRandomFiles();
    checkFilesExist();
    readFromFiles();
    deleteFiles();
}

void loop()
{
    delay(1000);
}

