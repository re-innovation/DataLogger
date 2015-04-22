/*
 * app.data.cpp
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
    temp1Conversion,
    temp2Conversion,
    temp3Conversion
};

static uint16_t s_fieldCount;

static bool s_debugOut = true;

static void debugTaskFn(void)
{
    uint8_t field = 0;
    uint8_t i;

    for (i = 0; i < s_fieldCount; i++)
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

        if (!lastinloop(i, s_fieldCount))
        {
            Serial.print(", ");                
        }
    }

    Serial.println();
}
static TaskAction debugTask(debugTaskFn, 1000, INFINITE_TICKS);

static void averageAndStoreTaskFn(void)
{
    uint8_t field = 0;
    uint8_t i;

    for (i = 0; i < s_fieldCount; i++)
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

void APP_DATA_Setup(unsigned long msInterval,
	uint16_t fieldCount, uint16_t averagerSize, uint16_t dataFieldBufferSize, FIELD_TYPE fieldTypes[])
{
	s_fieldCount = fieldCount;

	uint8_t i;
	
    s_debugAveragers = new Averager<uint16_t>*[fieldCount];
	s_averagers = new Averager<uint16_t>*[fieldCount];

	s_dataManager = new DataFieldManager();

	for (i = 0; i < fieldCount; ++i)
	{
        // Create a new numeric field and add to the manager
        DataField * field = new NumericDataField(fieldTypes[i], dataFieldBufferSize);

        if (field)
        {
            s_dataManager->addField(field);
        }
        else
        {
            Serial.print("Failed to create datafield of size ");
            Serial.print(dataFieldBufferSize);
            Serial.print(" and type ");
            Serial.println(fieldTypes[i]);
        }
        
        s_averagers[i] = new Averager<uint16_t>(averagerSize);
        s_debugAveragers[i] = new Averager<uint16_t>(10);

        if (!s_averagers[i])
        {
            Serial.print("Failed to create averager of size ");
            Serial.println(averagerSize);
        }
	}

	averageAndStoreTask.SetInterval(msInterval);
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
	return s_fieldCount;
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
