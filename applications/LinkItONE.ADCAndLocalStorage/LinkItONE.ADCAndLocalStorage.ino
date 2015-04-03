/*
 * LinkItONE.ADCAndLocalStorage
 *
 * Example application for the LinkIt ONE
 *
 * See README.md for full details
 *
 * Basic summary:
 *
 * - Reads data from ADS1115 ADCs (addresses 0x48, 0x49, 0x4A)
 * - Averages data every second
 * - Stores data to SD card every minute
 */

/*
 * Standard Library Includes
 */

#include <stdint.h>

/*
 * Arduino Library Includes
 */

#include <Wire.h>

/*
 * LinkIt One Includes
 */

#include <LSD.h>
#include <LGPS.h>
#include <LDateTime.h>

/*
 * DataLogger Includes
 */

#include "DLUtility.Time.h"
#include "DLTime.h"
#include "DLGPS.h"
#include "DLFilename.h"
#include "DLLocalStorage.h"
#include "DLUtility.h"
#include "DLLocation.h"
#include "DLSettings.h"
#include "DLDataField.h"
#include "DLSensor.ADS1x1x.h"

#include "TaskAction.h"

// Pointers to fuctionality objects

static ADS1115 s_ADCs[] = {
    ADS1115(0x48),
    ADS1115(0x49),
    ADS1115(0x4A)
};

// Short term (1 second) storage
static Averager<uint16_t> s_averagers[] = {
    Averager<uint16_t>(1000),
    Averager<uint16_t>(1000),
    Averager<uint16_t>(1000),
    Averager<uint16_t>(1000),
    Averager<uint16_t>(1000),
    Averager<uint16_t>(1000),
    Averager<uint16_t>(1000),
    Averager<uint16_t>(1000),
    Averager<uint16_t>(1000),
    Averager<uint16_t>(1000),
    Averager<uint16_t>(1000),
    Averager<uint16_t>(1000)
};

// Medium term (1 minute) in-RAM data storage
static NumericDataField<float> s_dataFields[] = {
    NumericDataField<float>(VOLTAGE, 60),
    NumericDataField<float>(VOLTAGE, 60),
    NumericDataField<float>(VOLTAGE, 60),
    NumericDataField<float>(VOLTAGE, 60),
    NumericDataField<float>(CURRENT, 60),
    NumericDataField<float>(CURRENT, 60),
    NumericDataField<float>(CURRENT, 60),
    NumericDataField<float>(CURRENT, 60),
    NumericDataField<float>(CURRENT, 60),
    NumericDataField<float>(CURRENT, 60),
    NumericDataField<float>(CURRENT, 60),
    NumericDataField<float>(CURRENT, 60)
};

static LocalStorageInterface * s_sdCard;
static  FILE_HANDLE s_fileHandle;

static uint16_t s_storedDataCount = 0;
static uint16_t s_adcReadCount = 0;

void averageAndStoreADCData(void)
{
    Serial.print("Averaging ADC data (");
    Serial.print(millis());
    Serial.println(")");

    s_storedDataCount++;
    uint8_t field = 0;
    uint8_t i;
    for (i = 0; i < 12; i++)
    {
        uint16_t average = s_averagers[i].getAverage();
        s_dataFields[i].storeData( (float)average );
    }
}

void writeToSDCard(void)
{
    Serial.println("Writing averages to SD card");
    uint8_t field = 0;
    uint8_t i, j;
    char buffer[10];

    openDataFileForToday();

    if (s_fileHandle != INVALID_HANDLE)
    {
        for (i = 0; i < 60; ++i)
        {
            for (j = 0; j < 12; ++j)
            {
                // Write from datafield to buffer then from buffer to SD file
                s_dataFields[j].getDataAsString(buffer, "%.4f", i);

                s_sdCard->write(s_fileHandle, buffer);

                if (!lastinloop(j, 12))
                {
                    s_sdCard->write(s_fileHandle, ", ");
                }
            }
            s_sdCard->write(s_fileHandle, "\n");
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

TaskAction readFromADCsTask(readFromADCsTaskFn, 100, INFINITE_TICKS);
void readFromADCsTaskFn(void)
{
    s_adcReadCount++;
    uint8_t adc = 0;
    uint8_t ch = 0;
    uint8_t field = 0;
    for (adc = 0; adc < 3; adc++)
    {
        for (ch = 0; ch < 4; ch++)
        {
            field = (adc*4)+ch;
            s_averagers[field].newData(s_ADCs[adc].readADC_SingleEnded(ch));
        }
    }
}

static TM s_time;

static void openDataFileForToday(void)
{

    Time_GetTime(&s_time, TIME_PLATFORM);
    Filename_setFromDate(s_time.tm_mday, s_time.tm_mon, s_time.tm_year);

    char const * const pFilename = Filename_get();

    if (s_sdCard->fileExists(pFilename))
    {
        s_fileHandle = s_sdCard->openFile(pFilename, true);
    }
    else
    {
        s_fileHandle = s_sdCard->openFile(pFilename, true);
        char csvHeaders[200];
        DataField_writeHeadersToBuffer(csvHeaders, s_dataFields, 12, 200);
        s_sdCard->write(s_fileHandle, csvHeaders);
    }
}

void setup()
{
    // setup Serial port
    Serial.begin(115200);

    delay(10000);

    uint8_t i = 0;
    for (i = 0; i < 3; i++)
    {
        s_ADCs[i].begin();
    }

    s_sdCard = LocalStorage_GetLocalStorageInterface(LINKITONE_SD_CARD);
}

void loop()
{
    readFromADCsTask.tick();

    if (s_adcReadCount == 10)
    {
        s_adcReadCount = 0;
        averageAndStoreADCData();

        if (s_storedDataCount == 60)
        {
            s_storedDataCount = 0;
            writeToSDCard();
        }
    }
}
