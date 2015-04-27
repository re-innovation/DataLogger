/*
 * app.sd_storage.cpp
 *
 * James Fowkes
 *
 * 06 April 2015
 *
 * Handles SD card storage for the LinkIt ONE ADCAndLocalStorage example
 */

/*
 * Arduino Library Includes
 */

#include <arduino.h>
 
/*
 * Standard Library Includes
 */

#include <stdint.h>

/*
 * Datalogger Library Includes
 */

#include "DLFilename.h"
#include "DLLocalStorage.h"
#include "DLUtility.h"
#include "DLDataField.Types.h"
#include "DLDataField.h"
#include "DLDataField.Manager.h"
#include "DLUtility.Time.h"
#include "DLTime.h"
#include "DLCSV.h"
#include "DLSettings.h"
#include "DLSettings.Global.h"
#include "DLSettings.Reader.Errors.h"
#include "DLSettings.Global.Reader.h"
#include "DLSettings.DataChannels.Reader.h"

/*
 * Application Includes
 */

#include "app.h"
#include "app.sd_storage.h"
#include "app.data.h"
#include "TaskAction.h"


static LocalStorageInterface * s_sdCard;
static FILE_HANDLE s_fileHandle;
static uint32_t s_entryID = 0;

static TM s_time;
static TM s_lastSDTimestamp;

static char s_directory[] = "Datalogger";
static char s_filePath[100];

static char s_errString[128];

static bool s_debugThisModule = false;

static void localPrintFn(char const * const toPrint)
{
    Serial.print(toPrint);
}

static void writeToSDCardTaskFn(void)
{
    uint8_t i;
    char buffer[10];

    NumericDataField * pField;
    uint16_t nFields = APP_DATA_GetNumberOfFields();
    
    if (s_debugThisModule)
    {       
        Serial.print("Writing averages to SD card (");
        Serial.print(nFields);
        Serial.println("fields).");
    }

    APP_SD_OpenDataFileForToday();

    if (s_fileHandle != INVALID_HANDLE)
    {
        APP_SD_WriteTimestampToOpenFile();
        APP_SD_WriteEntryIDToOpenFile();
            
        for (i = 0; i < nFields; ++i)
        {
            pField = APP_Data_GetField(i);
            // Write from datafield to buffer then from buffer to SD file
            pField->getDataAsString(buffer, "%.4f", i);

            s_sdCard->write(s_fileHandle, buffer);

            if (!lastinloop(i, nFields))
            {
                s_sdCard->write(s_fileHandle, ", ");
            }
        }
        s_sdCard->write(s_fileHandle, "\r\n");
        s_sdCard->closeFile(s_fileHandle);
    }
    else
    {
        if (s_debugThisModule)
        {
            Serial.print("Could not open '");
            Serial.print(Filename_get());
            Serial.println("' when trying to write data.");
        }
    }
}

static TaskAction writeToSDCardTask(writeToSDCardTaskFn, 0, INFINITE_TICKS);

void APP_SD_Init(void)
{
    s_sdCard = LocalStorage_GetLocalStorageInterface(LINKITONE_SD_CARD);
}

void APP_SD_Setup(unsigned long msInterval)
{
	Time_GetTime(&s_lastSDTimestamp, TIME_PLATFORM);
    s_sdCard->mkDir(s_directory);
    writeToSDCardTask.SetInterval(msInterval);
    writeToSDCardTask.ResetTime();
}

void createAndOpenNewFileForToday(void)
{
    char const * const pFilename = Filename_get();

    if (s_debugThisModule)
    {
        Serial.print("Creating new file: ");
        Serial.println(pFilename);
    }

    sprintf(s_filePath, "%s/%s", s_directory, pFilename);    
    s_fileHandle = s_sdCard->openFile(s_filePath, true);

    if (s_fileHandle == INVALID_HANDLE && s_debugThisModule)
    {
        Serial.print("Failed to create ");
        Serial.println(pFilename);
    }

    // Write Timestamp and Entry ID headers
    s_sdCard->write(s_fileHandle, "Timestamp, Entry ID, ");

    char csvHeaders[200];
    APP_DATA_WriteHeadersToBuffer(csvHeaders, 200);

    s_sdCard->write(s_fileHandle, csvHeaders);
    s_entryID = 0;
}

void APP_SD_OpenDataFileForToday(void)
{
    Time_GetTime(&s_time, TIME_PLATFORM);
    Filename_setFromDate(s_time.tm_mday, s_time.tm_mon, s_time.tm_year, 0);

    char const * const pFilename = Filename_get();

    sprintf(s_filePath, "%s/%s", s_directory, pFilename);    
    
    if (s_sdCard->fileExists(s_filePath))
    {
        if (s_debugThisModule)
        {
            Serial.print("Opening existing file: ");
            Serial.println(pFilename);
        }
        s_fileHandle = s_sdCard->openFile(s_filePath, true);
    }
    else
    {
        createAndOpenNewFileForToday();
    }
}

void APP_SD_WriteTimestampToOpenFile(void)
{
    char buffer[30];
    time_increment_seconds(&s_lastSDTimestamp);
    CSV_writeTimestampToBuffer(&s_lastSDTimestamp, buffer);
    s_sdCard->write(s_fileHandle, buffer);
    s_sdCard->write(s_fileHandle, ", ");
}

void APP_SD_WriteEntryIDToOpenFile(void)
{
    char buffer[30];
    s_entryID++;
    sprintf(buffer, "%d, ", s_entryID);
    s_sdCard->write(s_fileHandle, buffer);
}

/*void APP_SD_ReadAllDataFromCurrentFile(char * buffer, uint32_t maxSize)
{
    char const * const pFilename = Filename_get();
    uint32_t c = 0;

    sprintf(s_filePath, "%s/%s", s_directory, pFilename);    

    if (s_sdCard->fileExists(s_filePath))
    {
        s_fileHandle = s_sdCard->openFile(s_filePath, false);

        // Discard the first line
        s_sdCard->readLine(s_fileHandle,  buffer, maxSize, false);

        while (!s_sdCard->endOfFile(s_fileHandle))
        {
            c += s_sdCard->readLine(s_fileHandle, &buffer[c], maxSize-c, false);
        }

        s_sdCard->closeFile(s_fileHandle);
        
        Serial.println("Read from file:");
        Serial.print(buffer);
        Serial.println("");
    }
    else
    {
        Serial.print("Could not open ");
        Serial.print(pFilename);
        Serial.println(" for reading.");
    }
}*/

void APP_SD_ReadGlobalSettings(char const * const filename)
{
    SETTINGS_READER_RESULT result = Settings_readGlobalFromFile(s_sdCard, filename);

    if (result != ERR_READER_NONE)
    {
        APP_FatalError(Settings_getLastReaderResultText());
    }

    Settings_echoAllSet(localPrintFn);

    if (!Settings_allRequiredSettingsRead())
    {
        char missingSettings[128];
        Settings_getMissingNames(missingSettings, 128);
        sprintf(s_errString, "Some or all required settings not found in '%s': %s",
            filename, missingSettings);

        APP_FatalError(s_errString);
    }

    if (Settings_stringIsSet(DEBUG_MODULES))
    {
        APP_SetDebugModules(Settings_getString(DEBUG_MODULES));
    }
}

void APP_SD_ReadDataChannelSettings(DataFieldManager * pManager, char const * const filename)
{
    if (ERR_READER_NONE != Settings_readChannelsFromFile(pManager, s_sdCard, filename))
    {
        APP_FatalError(Settings_getLastReaderResultText());
    }
}

void APP_SD_EnableDebugging(void)
{
    s_debugThisModule = true;
    s_sdCard->setEcho(true);
}

void APP_SD_Tick(void)
{
	writeToSDCardTask.tick();
}