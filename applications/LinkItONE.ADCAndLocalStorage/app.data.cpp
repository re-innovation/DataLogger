/*
 * app.data.cpp
 *
 * James Fowkes
 *
 * 06 April 2015
 *
 * Handles SD card storage for the ADCAndLocalStorage example
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

static DataFieldManager * s_dataManager;
static DataFieldManager * s_dataDebugManager;

static bool s_debugOut = true;

static bool s_setupValid = false;

/*
 * calculateNumberOfAveragesToStore
 *
 * The storage interval will not always be an integer multiple of the averaging interval.
 * For example, the averaging interval may be 2 seconds and the storage interval may be 15 seconds.
 * In this case, the number of averages that needs to be stored will be 7 or 8. This function returns
 * the correct (highest possible) number of averages that need to be stored)
 */
static uint16_t calculateNumberOfAveragesToStore(uint16_t storageInterval, uint16_t averagingInterval)
{
    if (storageInterval % averagingInterval == 0)
    {
        return storageInterval / averagingInterval;
    }
    else
    {
        return (storageInterval / averagingInterval) + 1;
    }
}

static void debugTaskFn(void)
{
    uint8_t i;
    uint8_t fieldCount = s_dataDebugManager->fieldCount();

    for (i = 0; i < fieldCount; i++)
    {
        float average = ((NumericDataField *)s_dataDebugManager->getField(i))->getRawData(0);
        float toShow = ((NumericDataField *)s_dataDebugManager->getField(i))->getConvData(0);

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
static TaskAction debugTask(debugTaskFn, 1000, INFINITE_TICKS);

/*
 * Public Functions
 */

void APP_Data_Setup(
    unsigned long averagingInterval, uint16_t valuesPerSecond,
    uint16_t storageInterval, char const * const filename)
{    
    if (averagingInterval == 0) { APP_FatalError("Averaging interval is 0 seconds!"); }

    if (averagingInterval > storageInterval)
    {
        APP_FatalError("Averaging interval cannot be longer than storage interval.");
    }

    uint32_t averagerSize = valuesPerSecond * averagingInterval;
    uint32_t numberOfAveragesToStore = calculateNumberOfAveragesToStore(storageInterval, averagingInterval);

    // Create two data managers, one for actual data and one for debugging
    
    s_dataManager = new DataFieldManager(numberOfAveragesToStore, averagerSize);
    s_dataDebugManager = new DataFieldManager(1, averagerSize);

    if (!s_dataManager) { APP_FatalError("Failed to create datafield manager"); }
    if (!s_dataDebugManager) { APP_FatalError("Failed to create debug manager"); }

    APP_SD_ReadDataChannelSettings(s_dataManager, filename);
    APP_SD_ReadDataChannelSettings(s_dataDebugManager, filename);

    uint32_t count = s_dataManager->fieldCount();
    Serial.print("Datamanager created with ");
    Serial.print(count);
    Serial.println(" fields.");

    uint32_t i;

    for (i = 0; i < count; i++)
    {
        Serial.print("Field ");
        Serial.print(i);
        Serial.print(" type is ");
        Serial.print(s_dataManager->getField(i)->getTypeString());
        Serial.println(".");
    }

    if (s_debugOut)
    {
        debugTask.ResetTime();
    }

    s_setupValid = true;
}

void APP_Data_NewData(int32_t data, uint16_t field)
{
    char errBuffer[64];
    NumericDataField* pField;

    pField =  (NumericDataField*)(s_dataManager->getField(field));
    if (!pField)
    {
        sprintf(errBuffer, "Attempt to add data to non-existent field %d", field);
        APP_FatalError(errBuffer);
    }
    pField->storeData( data );
    
    pField = (NumericDataField*)(s_dataDebugManager->getField(field));
    if (!pField)
    {
        sprintf(errBuffer, "Attempt to add data to non-existent debug field %d", field);
        APP_FatalError(errBuffer);
    }
    pField->storeData( data );
}

uint16_t APP_Data_GetNumberOfFields(void)
{
    return s_dataManager->fieldCount();
}

void APP_Data_WriteHeadersToBuffer(char * buffer, uint8_t bufferLength)
{
    (void)s_dataManager->writeHeadersToBuffer(buffer, bufferLength);
}

NumericDataField * APP_Data_GetField(uint8_t i)
{
    return (NumericDataField *)s_dataManager->getField(i);
}

void APP_Data_Tick(void)
{
    if (s_setupValid)
    {
        if (s_debugOut)
        {
            debugTask.tick();
        }
    }
}
