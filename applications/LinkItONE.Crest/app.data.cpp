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
#include "app.sms.h"

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

static bool s_debugOut = true;
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

void APP_DATA_Setup(
    unsigned long storageAveragingInterval, unsigned long uploadAveragingInterval,
    uint16_t valuesPerSecond, uint16_t storageInterval, uint16_t uploadInterval, char const * const filename)
{    
    if (storageAveragingInterval == 0) { APP_FatalError("Storage averaging interval is 0 seconds!"); }
    if (uploadAveragingInterval == 0) { APP_FatalError("Upload averaging interval is 0 seconds!"); }

    if (storageAveragingInterval > storageInterval)
    {
        APP_FatalError("Storage averaging interval cannot be longer than storage interval.");
    }

    if (uploadAveragingInterval > uploadInterval)
    {
        APP_FatalError("Upload averaging interval cannot be longer than upload interval.");
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

    if (!s_storageManager) { APP_FatalError("Failed to create storage manager"); }
    if (!s_uploadManager) { APP_FatalError("Failed to create upload manager"); }
    if (!s_dataDebugManager) { APP_FatalError("Failed to create debug manager"); }

    uint8_t storageFieldCount = APP_SD_ReadDataChannelSettings(s_storageManager, filename);    
    uint8_t uploadFieldCount = APP_SD_ReadDataChannelSettings(s_uploadManager, filename);
    uint8_t debugFieldCount = APP_SD_ReadDataChannelSettings(s_dataDebugManager, filename);

    if ((storageFieldCount != uploadFieldCount) || (uploadFieldCount != debugFieldCount) || (debugFieldCount != storageFieldCount))
    {
        APP_FatalError("Field counts for data managers do not match!");
    }
    else
    {
        s_fieldCount = storageFieldCount;
    }

    if (s_fieldCount == 0)
    {
        APP_FatalError("No valid channel configurations read!");
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

    if (s_debugOut)
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

void APP_DATA_NewDataArray(int32_t * data)
{
    s_storageManager->storeDataArray(data);
    s_uploadManager->storeDataArray(data);
    s_dataDebugManager->storeDataArray(data);
}

void APP_DATA_GetUploadData(float * buffer)
{
    s_uploadManager->getDataArray(buffer, true, true);
}

uint16_t APP_DATA_GetNumberOfFields(void)
{
    return s_fieldCount;
}

bool APP_DATA_StorageDataRemaining(void)
{
    return s_storageManager->hasData();
}

bool APP_DATA_UploadIsPending(void)
{
    return s_uploadPending;
}

bool APP_DATA_UploadDataRemaining(void)
{
    return s_uploadManager->count() > 0;
}

void APP_DATA_WriteHeadersToBuffer(char * buffer, uint8_t bufferLength)
{
    (void)s_storageManager->writeHeadersToBuffer(buffer, bufferLength);
}

uint32_t * APP_DATA_GetChannelNumbers(void)
{
    return s_storageManager->getChannelNumbers();
}

uint32_t APP_DATA_GetNumberOfAveragesForStorage(void) { return s_numberOfAveragesToStore; }
uint32_t APP_DATA_GetNumberOfAveragesForUpload(void) { return s_numberOfAveragesToUpload; }

uint32_t APP_DATA_GetUploadBufferSize(void)
{
    uint32_t uploadBufferSize = 0;
    uploadBufferSize = 10 * APP_DATA_GetNumberOfFields(); // Allow 10 bytes per field
    uploadBufferSize += 20; // Allow 20 extra chars per data line
    uploadBufferSize *= APP_DATA_GetNumberOfAveragesForUpload(); // Each line needs recording
    uploadBufferSize *= 2; // For safety, allocate twice the requirement
    uploadBufferSize = max(uploadBufferSize, 512); // Ensure a minimum size for the buffer
    return uploadBufferSize;
}

NumericDataField * APP_DATA_GetUploadField(uint8_t i)
{
    return (NumericDataField *)s_uploadManager->getField(i);
}

NumericDataField * APP_DATA_GetStorageField(uint8_t i)
{
    return (NumericDataField *)s_storageManager->getField(i);
}

void APP_DATA_SetUploadPending(bool pending)
{
    s_uploadPending = pending;
}

void APP_DATA_Tick(void)
{
    if (s_setupValid)
    {
        if (s_debugOut)
        {
            debugTask.tick();
        }
    }
}
