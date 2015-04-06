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

/*
 * Application Includes
 */

#include "app.sd_storage.h"
#include "app.data.h"

#include "TaskAction.h"

static LocalStorageInterface * s_sdCard;
static FILE_HANDLE s_fileHandle;
static uint32_t s_entryID = 0;
static uint16_t s_fileCountToday = 0;

static uint16_t s_linesPerFile;

static TM s_time;
static TM s_lastSDTimestamp;

static void writeToSDCardTaskFn(void)
{
    uint8_t field = 0;
    uint8_t i, j;
    char buffer[10];

    NumericDataField<float> ** dataFields = APP_DATA_GetDataFieldsPtr();
    uint16_t nFields = APP_DATA_GetNumberOfFields();

    Serial.print("Writing averages to SD card (");
    Serial.print(s_linesPerFile);
    Serial.print(" lines with ");
    Serial.print(nFields);
    Serial.println(" fields each).");
    APP_SD_OpenDataFileForToday();

    if (s_fileHandle != INVALID_HANDLE)
    {
        for (i = 0; i < s_linesPerFile; ++i)
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
        }

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

void APP_SD_Setup(unsigned long msInterval, uint16_t linesPerFile)
{
	s_linesPerFile = linesPerFile;

	Time_GetTime(&s_lastSDTimestamp, TIME_PLATFORM);
	s_sdCard = LocalStorage_GetLocalStorageInterface(LINKITONE_SD_CARD);
    s_sdCard->setEcho(true);
    writeToSDCardTask.SetInterval(msInterval);
    writeToSDCardTask.ResetTime();
}

void APP_SD_CreateNewDataFile(void)
{
    APP_SD_SetNewFilename();

    char const * const pFilename = Filename_get();

    Serial.print("Creating new file: ");
    Serial.println(pFilename);

    s_fileHandle = s_sdCard->openFile(pFilename, true);

    // Write Timestamp and Entry ID headers
    s_sdCard->write(s_fileHandle, "Timestamp, Entry ID, ");

    char csvHeaders[200];
    NumericDataField<float> ** dataFields = APP_DATA_GetDataFieldsPtr();
    uint16_t nFields = APP_DATA_GetNumberOfFields();
    DataField_writeHeadersToBuffer(csvHeaders, dataFields, nFields, 200);
    s_sdCard->write(s_fileHandle, csvHeaders);
    s_entryID = 0;
    s_sdCard->closeFile(s_fileHandle);
}

void APP_SD_OpenDataFileForToday(void)
{

    char const * const pFilename = Filename_get();

    if (s_sdCard->fileExists(pFilename))
    {
        Serial.print("Opening existing file: ");
        Serial.println(pFilename);

        s_fileHandle = s_sdCard->openFile(pFilename, true);
    }
    else
    {
        Serial.print("Expected file '");
        Serial.print(pFilename);
        Serial.print(" 'does not exist.");
    }
}

void APP_SD_SetNewFilename(void)
{
    Time_GetTime(&s_time, TIME_PLATFORM);

    s_fileCountToday = 0;
    Filename_setFromDate(s_time.tm_mday, s_time.tm_mon, s_time.tm_year, s_fileCountToday);

    while (s_sdCard->fileExists(Filename_get()))
    {
        Filename_setFromDate(s_time.tm_mday, s_time.tm_mon, s_time.tm_year, ++s_fileCountToday);
    }

    Serial.print("Starting with file index ");
    Serial.println(s_fileCountToday);
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

void APP_SD_ReadAllDataFromCurrentFile(char * buffer, uint32_t maxSize)
{
    char const * const pFilename = Filename_get();
    uint32_t c = 0;

    if (s_sdCard->fileExists(pFilename))
    {
        s_fileHandle = s_sdCard->openFile(pFilename, false);

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
}

void APP_SD_Tick(void)
{
	writeToSDCardTask.tick();
}