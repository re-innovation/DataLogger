/*
 * app.sd_storage.cpp
 *
 * James Fowkes
 *
 * 06 April 2015
 *
 * Handles SD card storage for the Crest PV application
 */

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
#include "DLDataField.h"
#include "DLUtility.Time.h"
#include "DLTime.h"
#include "DLCSV.h"
#include "DLSettings.h"
#include "DLSettings.Reader.h"

/*
 * Application Includes
 */

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

static char s_settingsFilename[] = "Datalogger.Settings";

static void writeToSDCardTaskFn(void)
{
    uint8_t field = 0;
    uint8_t i, j;
    char buffer[10];

    NumericDataField ** dataFields = APP_DATA_GetDataFieldsPtr();
    uint16_t nFields = APP_DATA_GetNumberOfFields();

    Serial.print("Writing averages to SD card (");
    Serial.print(nFields);
    Serial.println("fields).");

    APP_SD_OpenDataFileForToday();

    if (s_fileHandle != INVALID_HANDLE)
    {
        APP_SD_WriteTimestampToOpenFile();
        APP_SD_WriteEntryIDToOpenFile();
            
        for (j = 0; j < nFields; ++j)
        {
            // Write from datafield to buffer then from buffer to SD file
            dataFields[j]->getDataAsString(buffer, "%.4f", i);

            s_sdCard->write(s_fileHandle, buffer);

            if (!lastinloop(j, nFields))
            {
                s_sdCard->write(s_fileHandle, ", ");
            }
        }
        s_sdCard->write(s_fileHandle, "\r\n");
        s_sdCard->closeFile(s_fileHandle);
    }
    else
    {
        Serial.print("Could not open '");
        Serial.print(Filename_get());
        Serial.println("' when trying to write data.");
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

void createNewFileForToday(void)
{
    char const * const pFilename = Filename_get();

    Serial.print("Creating new file: ");
    Serial.println(pFilename);

    sprintf(s_filePath, "%s/%s", s_directory, pFilename);    
    s_fileHandle = s_sdCard->openFile(s_filePath, true);

    if (s_fileHandle == INVALID_HANDLE)
    {
        Serial.print("Failed to create ");
        Serial.println(pFilename);
    }

    // Write Timestamp and Entry ID headers
    s_sdCard->write(s_fileHandle, "Timestamp, Entry ID, ");

    char csvHeaders[200];
    NumericDataField ** dataFields = APP_DATA_GetDataFieldsPtr();
    uint16_t nFields = APP_DATA_GetNumberOfFields();
    DataField_writeHeadersToBuffer(csvHeaders, dataFields, nFields, 200);
    s_sdCard->write(s_fileHandle, csvHeaders);
    s_entryID = 0;
    s_sdCard->closeFile(s_fileHandle);
}

void APP_SD_OpenDataFileForToday(void)
{
    Time_GetTime(&s_time, TIME_PLATFORM);
    Filename_setFromDate(s_time.tm_mday, s_time.tm_mon, s_time.tm_year, 0);

    char const * const pFilename = Filename_get();

    sprintf(s_filePath, "%s/%s", s_directory, pFilename);    
    
    if (s_sdCard->fileExists(s_filePath))
    {
        Serial.print("Opening existing file: ");
        Serial.println(pFilename);

        s_fileHandle = s_sdCard->openFile(s_filePath, true);
    }
    else
    {
        createNewFileForToday();
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

void APP_SD_ReadSettings(void)
{
    char lineBuffer[100];
    
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

    // Echo out integer settings
    Serial.println("Integer Settings:");
    int i;
    for (i = 0; i < INT_SETTINGS_COUNT; i++)
    {
        Serial.print(Settings_getIntName((INTSETTING)i));
        Serial.print(": ");
        Serial.println(Settings_getInt((INTSETTING)i));
    }

    // Echo out string settings
    Serial.println("String Settings:");
    for (i = 0; i < STRING_SETTINGS_COUNT; i++)
    {
        Serial.print(Settings_getStringName((STRINGSETTING)i));
        Serial.print(": ");
        Serial.println(Settings_getString((STRINGSETTING)i));
    }

}

void APP_SD_ReadSettings(char * filename)
{
    char lineBuffer[100];
    if (s_sdCard->fileExists(filename))
    {
        Serial.print("Reading settings from '");
        Serial.print(filename);
        Serial.println("'.");
    }
    else
    {
        Serial.print("Settings file '");
        Serial.print(filename);
        Serial.println("' not found.");
        return;
    }

    uint8_t hndl = s_sdCard->openFile(filename, false);
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

    // Echo out integer settings
    Serial.println("Integer Settings:");
    int i;
    for (i = 0; i < INT_SETTINGS_COUNT; i++)
    {
        Serial.print(Settings_getIntName((INTSETTING)i));
        Serial.print(": ");
        Serial.println(Settings_getInt((INTSETTING)i));
    }

    // Echo out string settings
    Serial.println("String Settings:");
    for (i = 0; i < STRING_SETTINGS_COUNT; i++)
    {
        Serial.print(Settings_getStringName((STRINGSETTING)i));
        Serial.print(": ");
        Serial.println(Settings_getString((STRINGSETTING)i));
    }
}

void APP_SD_Tick(void)
{
	//writeToSDCardTask.tick();
}