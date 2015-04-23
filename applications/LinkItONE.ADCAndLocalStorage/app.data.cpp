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
#include "DLDataField.h"
#include "DLDataField.Manager.h"
#include "DLUtility.Time.h"
#include "DLTime.h"
#include "DLCSV.h"

/*
 * Application Includes
 */

#include "app.data.h"
#include "app.data_conversion.h"

#include "TaskAction.h"

/*
 * Applications Data
 */

static Averager<uint16_t> ** s_averagers;
static Averager<uint16_t> ** s_debugAveragers;

static DataFieldManager * s_dataManager;

static CONVERSION_FN s_conversionFunctions[] = 
{
    channel01Conversion,
    channel02Conversion,
    channel03Conversion,
    channel04Conversion,
    channel05Conversion,
    channel06Conversion,
    channel07Conversion,
    channel08Conversion,
    channel09Conversion,
    channel10Conversion,
    channel11Conversion,
    channel12Conversion,
};

static bool s_debugOut = true;

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
    uint8_t fieldCount = s_dataManager->count();

    for (i = 0; i < fieldCount; i++)
    {
        uint16_t average = s_debugAveragers[i]->getAverage();
    
        float toShow;
        if (s_conversionFunctions[i])
        {
            toShow = s_conversionFunctions[i](average); 
        }
        else
        {
            toShow = (float)average;
        }

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

static void averageAndStoreTaskFn(void)
{
    uint8_t i;

    for (i = 0; i < s_dataManager->count(); i++)
    {
        uint16_t average = s_averagers[i]->getAverage();

        float toStore;
        if (s_conversionFunctions[i])
        {
            toStore = s_conversionFunctions[i](average); 
        }
        else
        {
            toStore = (float)average;
        }

        ((NumericDataField*)s_dataManager->getField(i))->storeData( toStore );
    }
}
static TaskAction averageAndStoreTask(averageAndStoreTaskFn, 0, INFINITE_TICKS);

/*
 * Public Functions
 */

void APP_DATA_Setup(unsigned long averagingInterval,
    uint16_t fieldCount, uint16_t valuesPerSecond, uint16_t storageInterval, FIELD_TYPE fieldTypes[])
{
    uint8_t i;
    
    uint32_t averagerSize = valuesPerSecond * averagingInterval;
    uint32_t numberOfAveragesToStore = calculateNumberOfAveragesToStore(storageInterval, averagingInterval);

    s_debugAveragers = new Averager<uint16_t>*[fieldCount];
    s_averagers = new Averager<uint16_t>*[fieldCount];

    s_dataManager = new DataFieldManager();

    for (i = 0; i < fieldCount; ++i)
    {
        // Create a new numeric field and add to the manager
        DataField * field = new NumericDataField(fieldTypes[i], numberOfAveragesToStore);

        if (field)
        {
            s_dataManager->addField(field);
        }
        else
        {
            Serial.print("Failed to create datafield of size ");
            Serial.print(numberOfAveragesToStore);
            Serial.print(" and type ");
            Serial.println((int)fieldTypes[i]);
        }
        
        s_averagers[i] = new Averager<uint16_t>(averagerSize);
        s_debugAveragers[i] = new Averager<uint16_t>(valuesPerSecond);

        if (!s_averagers[i])
        {
            Serial.print("Failed to create averager of size ");
            Serial.println(averagerSize);
        }
    }

    averageAndStoreTask.SetInterval(averagingInterval * 1000);
    averageAndStoreTask.ResetTime();

    if (s_debugOut)
    {
        debugTask.ResetTime();
    }
}

void APP_DATA_NewData(uint16_t data, uint16_t field)
{
    s_averagers[field]->newData(data);
    s_debugAveragers[field]->newData(data);
}

uint16_t APP_DATA_GetNumberOfFields(void)
{
    return s_dataManager->count();
}

void APP_DATA_WriteHeadersToBuffer(char * buffer, uint8_t bufferLength)
{
    (void)s_dataManager->writeHeadersToBuffer(buffer, bufferLength);
}

NumericDataField * APP_Data_GetField(uint8_t i)
{
    return (NumericDataField *)s_dataManager->getField(i);
}

void APP_DATA_Tick(void)
{
    if (s_debugOut)
    {
        debugTask.tick();
    }
    averageAndStoreTask.tick();
}
