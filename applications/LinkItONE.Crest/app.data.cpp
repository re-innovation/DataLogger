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

static uint32_t s_numberOfAveragesToStore= 0;
static uint32_t s_numberOfAveragesToUpload = 0;

static bool s_debugOut = true;

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
    uint8_t fieldCount = s_dataDebugManager->count();

    while(s_dataDebugManager->hasData())
    {
        for (i = 0; i < fieldCount; i++)
        {
            float average = ((NumericDataField *)s_dataDebugManager->getField(i))->getRawData(false);
            float toShow = ((NumericDataField *)s_dataDebugManager->getField(i))->getConvData(true);

            Serial.print(toShow);
            Serial.print("(");
            Serial.print(average);
            Serial.print(")");

            if (!lastinloop(i, fieldCount))
            {
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
    unsigned long averagingInterval, uint16_t valuesPerSecond,
    uint16_t storageInterval, uint16_t uploadInterval, char const * const filename)
{    
    if (averagingInterval == 0) { APP_FatalError("Averaging interval is 0 seconds!"); }

    if (averagingInterval > storageInterval)
    {
        APP_FatalError("Averaging interval cannot be longer than storage interval.");
    }

    if (averagingInterval > uploadInterval)
    {
        APP_FatalError("Averaging interval cannot be longer than upload interval.");
    }

    uint32_t averagerSize = valuesPerSecond * averagingInterval;
    s_numberOfAveragesToStore = calculateNumberOfAverages(storageInterval, averagingInterval);
    s_numberOfAveragesToUpload = calculateNumberOfAverages(uploadInterval, averagingInterval);

    // Create three data managers, one for storing data, one for uploading data and one for debugging
    
    s_storageManager = new DataFieldManager(s_numberOfAveragesToStore, averagerSize);
    s_uploadManager = new DataFieldManager(s_numberOfAveragesToUpload, averagerSize);
    s_dataDebugManager = new DataFieldManager(1, averagerSize);

    if (!s_storageManager) { APP_FatalError("Failed to create storage manager"); }
    if (!s_uploadManager) { APP_FatalError("Failed to create upload manager"); }
    if (!s_dataDebugManager) { APP_FatalError("Failed to create debug manager"); }

    APP_SD_ReadDataChannelSettings(s_storageManager, filename);    
    APP_SD_ReadDataChannelSettings(s_uploadManager, filename);
    APP_SD_ReadDataChannelSettings(s_dataDebugManager, filename);

    uint32_t count = s_storageManager->count();
    Serial.print("Data Managers created with ");
    Serial.print(count);
    Serial.println(count > 1 ? " channels." : " channel.");
    
    uint32_t i;

    uint32_t * channelNumbers = s_storageManager->getChannelNumbers();

    for (i = 0; i < count; i++)
    {
        Serial.print("Channel ");
        Serial.print(channelNumbers[i]);
        Serial.print(" type is ");
        Serial.print(s_storageManager->getChannel(channelNumbers[i])->getTypeString());
        Serial.println(".");
    }

    if (s_debugOut)
    {
        debugTask.ResetTime();
    }

    s_setupValid = true;
}

void APP_DATA_NewData(int32_t data, uint16_t channel)
{
    char errBuffer[64];
    NumericDataField* pField;

    pField =  (NumericDataField*)(s_storageManager->getChannel(channel));
    if (!pField)
    {
        sprintf(errBuffer, "Attempt to add data to non-existent channel %d", channel);
        APP_FatalError(errBuffer);
    }
    pField->storeData( data );
    
    pField =  (NumericDataField*)(s_uploadManager->getChannel(channel));
    if (!pField)
    {
        sprintf(errBuffer, "Attempt to add data to non-existent channel %d", channel);
        APP_FatalError(errBuffer);
    }
    pField->storeData( data );

    pField = (NumericDataField*)(s_dataDebugManager->getChannel(channel));
    if (!pField)
    {
        sprintf(errBuffer, "Attempt to add data to non-existent debug field %d", channel);
        APP_FatalError(errBuffer);
    }
    pField->storeData( data );
}

uint16_t APP_DATA_GetNumberOfFields(void)
{
    return s_storageManager->count();
}

bool APP_DATA_StorageDataRemaining(void)
{
    return s_storageManager->hasData();
}

bool APP_DATA_UploadDataRemaining(void)
{
    return s_uploadManager->hasData();
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

NumericDataField * APP_Data_GetUploadField(uint8_t i)
{
    return (NumericDataField *)s_uploadManager->getField(i);
}

NumericDataField * APP_Data_GetStorageField(uint8_t i)
{
    return (NumericDataField *)s_storageManager->getField(i);
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
