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

static Averager<uint16_t> ** s_averagers;/* = {
    Averager<uint16_t>(ADC_READS_PER_SECOND * AVERAGING_PERIOD_SECONDS),
    Averager<uint16_t>(ADC_READS_PER_SECOND * AVERAGING_PERIOD_SECONDS),
    Averager<uint16_t>(ADC_READS_PER_SECOND * AVERAGING_PERIOD_SECONDS),
    Averager<uint16_t>(ADC_READS_PER_SECOND * AVERAGING_PERIOD_SECONDS),
    Averager<uint16_t>(ADC_READS_PER_SECOND * AVERAGING_PERIOD_SECONDS),
    Averager<uint16_t>(ADC_READS_PER_SECOND * AVERAGING_PERIOD_SECONDS),
    Averager<uint16_t>(ADC_READS_PER_SECOND * AVERAGING_PERIOD_SECONDS),
    Averager<uint16_t>(ADC_READS_PER_SECOND * AVERAGING_PERIOD_SECONDS),
    Averager<uint16_t>(ADC_READS_PER_SECOND * AVERAGING_PERIOD_SECONDS),
    Averager<uint16_t>(ADC_READS_PER_SECOND * AVERAGING_PERIOD_SECONDS),
    Averager<uint16_t>(ADC_READS_PER_SECOND * AVERAGING_PERIOD_SECONDS),
    Averager<uint16_t>(ADC_READS_PER_SECOND * AVERAGING_PERIOD_SECONDS)
};*/

static NumericDataField<float> ** s_dataFields;/* = {
    NumericDataField<float>(VOLTAGE, STORE_TO_SD_EVERY_N_AVERAGES),
    NumericDataField<float>(VOLTAGE, STORE_TO_SD_EVERY_N_AVERAGES),
    NumericDataField<float>(VOLTAGE, STORE_TO_SD_EVERY_N_AVERAGES),
    NumericDataField<float>(VOLTAGE, STORE_TO_SD_EVERY_N_AVERAGES),
    NumericDataField<float>(CURRENT, STORE_TO_SD_EVERY_N_AVERAGES),
    NumericDataField<float>(CURRENT, STORE_TO_SD_EVERY_N_AVERAGES),
    NumericDataField<float>(CURRENT, STORE_TO_SD_EVERY_N_AVERAGES),
    NumericDataField<float>(CURRENT, STORE_TO_SD_EVERY_N_AVERAGES),
    NumericDataField<float>(CURRENT, STORE_TO_SD_EVERY_N_AVERAGES),
    NumericDataField<float>(CURRENT, STORE_TO_SD_EVERY_N_AVERAGES),
    NumericDataField<float>(CURRENT, STORE_TO_SD_EVERY_N_AVERAGES),
    NumericDataField<float>(CURRENT, STORE_TO_SD_EVERY_N_AVERAGES),
    NumericDataField<float>(TEMPERATURE_C, STORE_TO_SD_EVERY_N_AVERAGES),
    NumericDataField<float>(TEMPERATURE_C, STORE_TO_SD_EVERY_N_AVERAGES),
    NumericDataField<float>(TEMPERATURE_C, STORE_TO_SD_EVERY_N_AVERAGES)
};*/

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

static uint16_t s_fieldCount;

static void averageAndStoreTaskFn(void)
{
    uint8_t field = 0;
    uint8_t i;

    for (i = 0; i < s_fieldCount; i++)
    {
        uint16_t average = s_averagers[i]->getAverage();
        s_averagers[i]->reset(NULL);

        float toStore;
        if (s_conversionFunctions[i])
        {
            toStore = s_conversionFunctions[i](average); 
        }
        else
        {
            toStore = (float)average;
        }
        s_dataFields[i]->storeData( toStore );
    }
    Serial.println("");
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
	
	s_averagers = new Averager<uint16_t>*[fieldCount];
	s_dataFields = new NumericDataField<float>*[fieldCount];

	for (i = 0; i < fieldCount; ++i)
	{
		s_averagers[i] = new Averager<uint16_t>(averagerSize);
		if (s_averagers[i])
		{
			Serial.print("Creating averager of size ");
			Serial.println(averagerSize);
		}

		s_dataFields[i] = new NumericDataField<float>(fieldTypes[i], dataFieldBufferSize);
		if (s_dataFields[i])
		{
			Serial.print("Creating datafield of size ");
			Serial.print(dataFieldBufferSize);
			Serial.print(" and type ");
			Serial.println(s_dataFields[i]->getTypeString());
		}
	}

	averageAndStoreTask.SetInterval(msInterval);
    averageAndStoreTask.ResetTime();
}

void APP_DATA_NewData(float data, uint16_t field)
{
	s_averagers[field]->newData(data);
}

NumericDataField<float> ** APP_DATA_GetDataFieldsPtr(void)
{
	return s_dataFields;
}

uint16_t APP_DATA_GetNumberOfFields(void)
{
	return s_fieldCount;
}

void APP_DATA_Tick(void)
{
	averageAndStoreTask.tick();
}