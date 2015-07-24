/*
 * app.data.cpp
 *
 * James Fowkes
 *
 * 06 April 2015
 *
 * Handles SD card storage for the CREST application
 */

/*
 * Arduino Library Includes
 */

#include <Arduino.h>

/*
 * Standard Library Includes
 */

#include <stdint.h>

/*
 * Datalogger Library Includes
 */

#include "DLError.h"

#include "DLFilename.h"
#include "DLLocalStorage.h"
#include "DLUtility.h"

#include "DLSettings.h"
#include "DLSettings.Global.h"

#include "DLDataField.Types.h"
#include "DLDataField.h"
#include "DLDataField.Manager.h"

#include "DLTime.h"
#include "DLCSV.h"
#include "TaskAction.h"

/*
 * Application Includes
 */

#include "app.h"
#include "app.data.h"
#include "app.sd_storage.h"

/*
 * Applications Data
 */

static DataFieldManager * s_storageManager = NULL;
static DataFieldManager * s_uploadManager = NULL;
static DataFieldManager * s_dataDebugManager = NULL;

static bool s_uploadPending = false;

static uint32_t s_numberOfAveragesToStore= 0;
static uint32_t s_numberOfAveragesToUpload = 0;
static uint8_t s_fieldCount;

static bool s_debugEnabled = true;
static bool * s_debugFieldFlags;

static bool s_setupValid = false;

/*
 * calculateNumberOfAverages
 *
 * The storage interval will not always be an integer multiple of the averaging interval.
 * For example, the averaging interval may be 2 seconds and the storage/upload interval may be 15 seconds.
 * In this case, the number of averages that needs to be stored will be 7 or 8. This function returns
 * the correct (highest possible) number of averages that need to be stored/uploaded)
 */
static uint16_t calculateNumberOfAverages(uint16_t interval, uint16_t averagingInterval)
{
    if (interval % averagingInterval == 0)
    {
        return interval / averagingInterval;
    }
    else
    {
        return (interval / averagingInterval) + 1;
    }
}

static void debugTaskFn(void)
{
    uint8_t i;
    uint8_t fieldCount = s_dataDebugManager->fieldCount();

    while(s_dataDebugManager->hasData())
    {
        for (i = 0; i < fieldCount; i++)
        {
            // Data has to be removed from manager whether it's printed or not
            float average = ((NumericDataField *)s_dataDebugManager->getField(i))->getRawData(false);
            float toShow = ((NumericDataField *)s_dataDebugManager->getField(i))->getConvData(true);
             
            if (s_debugFieldFlags[i])
            {   Serial.print(toShow);
                Serial.print("(");
                Serial.print(average);
                Serial.print(")");
                Serial.print(", ");
            }
        }
        Serial.println();
    }
}
static TaskAction debugTask(debugTaskFn, 1000, INFINITE_TICKS);

/*
 * Public Functions
 */

void APP_Data_Setup(
    unsigned long storageAveragingInterval, unsigned long uploadAveragingInterval,
    uint16_t valuesPerSecond, uint16_t storageInterval, uint16_t uploadInterval, char const * const filename)
{    
    if (storageAveragingInterval == 0) { Error_Fatal("Storage averaging interval is 0 seconds!", ERR_FATAL_CONFIG); }
    if (uploadAveragingInterval == 0) { Error_Fatal("Upload averaging interval is 0 seconds!", ERR_FATAL_CONFIG); }

    if (storageAveragingInterval > storageInterval)
    {
        Error_Fatal("Storage averaging interval cannot be longer than storage interval.", ERR_FATAL_CONFIG);
    }

    if (uploadAveragingInterval > uploadInterval)
    {
        Error_Fatal("Upload averaging interval cannot be longer than upload interval.", ERR_FATAL_CONFIG);
    }

    uint32_t storageAveragerSize = valuesPerSecond * storageAveragingInterval;
    uint32_t uploadAveragerSize = valuesPerSecond * uploadAveragingInterval;

    s_numberOfAveragesToStore = calculateNumberOfAverages(storageInterval, storageAveragingInterval);
    s_numberOfAveragesToUpload = calculateNumberOfAverages(uploadInterval, uploadAveragingInterval);

    Serial.print("Storing ");
    Serial.print(s_numberOfAveragesToStore);
    Serial.print(" averages of ");
    Serial.print(storageAveragerSize);
    Serial.print(" values (");
    Serial.print(valuesPerSecond);
    Serial.print(" values per second * ");
    Serial.print(storageAveragingInterval);
    Serial.println(" seconds).");

    Serial.print("Uploading ");
    Serial.print(s_numberOfAveragesToUpload);
    Serial.print(" averages of ");
    Serial.print(uploadAveragerSize);
    Serial.print(" values (");
    Serial.print(valuesPerSecond);
    Serial.print(" values per second * ");
    Serial.print(uploadAveragingInterval);
    Serial.println(" seconds).");

    // Create three data managers, one for storing data, one for uploading data and one for debugging
    
    s_storageManager = new DataFieldManager(s_numberOfAveragesToStore, storageAveragerSize);
    s_uploadManager = new DataFieldManager(s_numberOfAveragesToUpload, uploadAveragerSize);
    s_dataDebugManager = new DataFieldManager(1, valuesPerSecond);

    if (!s_storageManager) { Error_Fatal("Failed to create storage manager", ERR_FATAL_RUNTIME); }
    if (!s_uploadManager) { Error_Fatal("Failed to create upload manager", ERR_FATAL_RUNTIME); }
    if (!s_dataDebugManager) { Error_Fatal("Failed to create debug manager", ERR_FATAL_RUNTIME); }

    Serial.print("Attempting to read channel settings from ");
    Serial.print("filename");
    Serial.println("...");
    
    uint8_t storageFieldCount = APP_SD_ReadDataChannelSettings(s_storageManager, filename);    
    uint8_t uploadFieldCount = APP_SD_ReadDataChannelSettings(s_uploadManager, filename);
    uint8_t debugFieldCount = APP_SD_ReadDataChannelSettings(s_dataDebugManager, filename);

    if ((storageFieldCount != uploadFieldCount) || (uploadFieldCount != debugFieldCount) || (debugFieldCount != storageFieldCount))
    {
        Error_Fatal("Field counts for data managers do not match!", ERR_FATAL_CONFIG);
    }
    else
    {
        s_fieldCount = storageFieldCount;
    }

    if (s_fieldCount == 0)
    {
        Error_Fatal("No valid channel configurations read!", ERR_FATAL_CHANNEL);
    }

    Serial.print("Data Managers created with ");
    Serial.print(s_fieldCount);
    Serial.println(s_fieldCount > 1 ? " channels." : " channel.");
    
    uint32_t i;

    uint32_t * channelNumbers = s_storageManager->getChannelNumbers();
    char configString[100];

    for (i = 0; i < s_fieldCount; i++)
    {
        s_storageManager->getChannel(channelNumbers[i])->getConfigString(configString);
        Serial.print("Channel ");
        Serial.print(channelNumbers[i]);
        Serial.print(" type is ");
        Serial.print(s_storageManager->getChannel(channelNumbers[i])->getTypeString());
        Serial.print(". (");
        Serial.print(configString);
        Serial.println(")");
    }

    // Serial data output is enabled if ENABLE_DATA_DEBUG is not zero
    if (Settings_intIsSet(ENABLE_DATA_DEBUG))
    {
        s_debugEnabled = Settings_getInt(ENABLE_DATA_DEBUG) != 0;
    }
    else
    {
        s_debugEnabled = true; // If the setting is not present, default to enabled
    }
    
    if (s_debugEnabled)
    {
        /* Set the fields to debug based on settings */
        s_debugFieldFlags = new bool[s_fieldCount];

        // Assume all fields should be debugged
        fillArray(s_debugFieldFlags, (bool)true, s_fieldCount);

        if (Settings_stringIsSet(DEBUG_FIELDS))
        {
            Serial.print("Debugging fields: ");
            char const * const debugFields = Settings_getString(DEBUG_FIELDS);
            for (i = 0; i < s_fieldCount; i++)
            {
                char expected[3];
                sprintf(expected, "%02d", i+1); // Fields must be 01, 02 (not 1, 2)
                s_debugFieldFlags[i] = strstr(debugFields, expected);
                if (s_debugFieldFlags[i])
                {
                    Serial.print(i+1);
                    Serial.print(", ");
                }
            }
            Serial.println("");

        }
        else
        {
            Serial.println("Debugging all fields.");
        }
        debugTask.ResetTime();
    }

    s_setupValid = true;
}

void APP_Data_NewDataArray(int32_t * data)
{
    s_storageManager->storeDataArray(data);
    s_uploadManager->storeDataArray(data);
    s_dataDebugManager->storeDataArray(data);
}

void APP_Data_GetUploadData(float * buffer)
{
    s_uploadManager->getDataArray(buffer, true, true);
}

uint16_t APP_Data_GetNumberOfFields(void)
{
    return s_fieldCount;
}

bool APP_Data_StorageDataRemaining(void)
{
    return s_storageManager->hasData();
}

bool APP_Data_UploadIsPending(void)
{
    return s_uploadPending;
}

bool APP_Data_UploadDataRemaining(void)
{
    return s_uploadManager->count() > 0;
}

void APP_Data_WriteHeadersToBuffer(char * buffer, uint8_t bufferLength)
{
    (void)s_storageManager->writeHeadersToBuffer(buffer, bufferLength);
}

uint32_t * APP_Data_GetChannelNumbers(void)
{
    return s_storageManager->getChannelNumbers();
}

uint32_t APP_Data_GetNumberOfAveragesForStorage(void) { return s_numberOfAveragesToStore; }
uint32_t APP_Data_GetNumberOfAveragesForUpload(void) { return s_numberOfAveragesToUpload; }

uint32_t APP_Data_GetUploadBufferSize(void)
{
    uint32_t uploadBufferSize = 0;
    uploadBufferSize = 10 * APP_Data_GetNumberOfFields(); // Allow 10 bytes per field
    uploadBufferSize += 20; // Allow 20 extra chars per data line
    uploadBufferSize *= APP_Data_GetNumberOfAveragesForUpload(); // Each line needs recording
    uploadBufferSize *= 2; // For safety, allocate twice the requirement
    uploadBufferSize = max(uploadBufferSize, 512); // Ensure a minimum size for the buffer
    return uploadBufferSize;
}

NumericDataField * APP_Data_GetUploadField(uint8_t i)
{
    return (NumericDataField *)s_uploadManager->getField(i);
}

NumericDataField * APP_Data_GetStorageField(uint8_t i)
{
    return (NumericDataField *)s_storageManager->getField(i);
}

void APP_Data_SetUploadPending(bool pending)
{
    s_uploadPending = pending;
}

void APP_Data_Tick(void)
{
    if (s_setupValid)
    {
        if (s_debugEnabled)
        {
            debugTask.tick();
        }
    }
}
