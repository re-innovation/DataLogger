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

#include <LGPRS.h>

/*
 * DataLogger Includes
 */

#include "DLLocalStorage.h"
#include "DLUtility.h"
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

// In-RAM data storage
static DataField s_dataFields[] = {
    DataField(VOLTAGE),
    DataField(VOLTAGE),
    DataField(VOLTAGE),
    DataField(VOLTAGE),
    DataField(CURRENT),
    DataField(CURRENT),
    DataField(CURRENT),
    DataField(CURRENT),
    DataField(CURRENT),
    DataField(CURRENT),
    DataField(CURRENT),
    DataField(CURRENT)
};

static LocalStorageInterface * s_sdCard;

// Tasks
TaskAction averagingTask(averagingTaskFn, 1000, INFINITE_TICKS);
void averagingTaskFn(void)
{

}

TaskAction storageTask(storageTaskFn, 1000, INFINITE_TICKS);
void storageTaskFn(void)
{

}

void readFromADCs(void)
{
    uint8_t adc = 0;
    uint8_t ch = 0;
    uint8_t field = 0;
    for (adc = 0; adc < 3; adc++)
    {
        for (ch = 0; ch < 4; ch++)
        {
            field = (adc*4)+ch;
            s_dataFields[field].storeData(s_ADCs[adc].readADC_SingleEnded(ch));
        }
    }
}

void setup()
{
    // setup Serial port
    Serial.begin(115200);

    delay(10000);


    s_sdCard = LocalStorage_GetLocalStorageInterface(LINKITONE_SD_CARD);

    uint8_t i = 0;
    for (i = 0; i < 3; i++)
    {
        s_ADCs[i].begin();
    }
}

void loop()
{
    averagingTask.tick();
    storageTask.tick();


    // Read from ADCs constantly
    readFromADCs();
}
