/*
 * LinkItONE.ADCAndLocalStorage
 *
 * Example application for storing ADC values to LinkIt ONE SD Card
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
#include <LGPRS.h>
#include <LDateTime.h>

/*
 * DataLogger Includes
 */

#include "DLUtility.Time.h"
#include "DLTime.h"
#include "DLCSV.h"
#include "DLGPS.h"
#include "DLFilename.h"
#include "DLLocalStorage.h"
#include "DLUtility.h"
#include "DLLocation.h"
#include "DLSettings.h"
#include "DLSettings.Reader.h"
#include "DLDataField.h"
#include "DLSensor.ADS1x1x.h"
#include "DLSensor.LinkItONE.h"
#include "DLSensor.Thermistor.h"

#include "TaskAction.h"

/*
 * Application Includes
 */

#include "app.sd_storage.h"
#include "app.data.h"

// Pointers to fuctionality objects

static ADS1115 s_ADCs[] = {
    ADS1115(0x48),
    ADS1115(0x49),
    ADS1115(0x4A)
};

#define LED1_PIN (4)
#define LED2_PIN (5)

// There are 12 averages for voltage and current data
#define V_AND_I_COUNT (12)

#define FIELD_COUNT (V_AND_I_COUNT)

#define ADC_READS_PER_SECOND (10)
#define AVERAGING_PERIOD_SECONDS (30)

#define MS_PER_ADC_READ (1000 / ADC_READS_PER_SECOND)

// Medium term in-RAM data storage and remote storage
#define STORE_TO_SD_EVERY_N_AVERAGES (1)
#define STORE_TO_SD_INTERVAL_MS (STORE_TO_SD_EVERY_N_AVERAGES * AVERAGING_PERIOD_SECONDS * 1000)

/*
 * Tasks
 */

void heartbeatTaskFn(void)
{
    static bool ledState = false;
    digitalWrite(LED2_PIN, ledState ? HIGH : LOW);
    ledState = !ledState;
}
TaskAction heartbeatTask(heartbeatTaskFn, 500, INFINITE_TICKS);

void readFromADCsTaskFn(void)
{
    uint8_t adc = 0;
    uint8_t ch = 0;
    uint8_t field = 0;

    // Read the ADC1x1x ICs for fields 1 - 12
    for (adc = 0; adc < 3; adc++)
    {
        for (ch = 0; ch < 4; ch++)
        {
            field = (adc*4)+ch;
            uint16_t data = s_ADCs[adc].readADC_SingleEnded(ch);
            APP_DATA_NewData(data, field);
        }
    }
}
TaskAction readFromADCsTask(readFromADCsTaskFn, MS_PER_ADC_READ, INFINITE_TICKS);

void setup()
{
    // setup Serial port
    Serial.begin(115200);

    delay(10000);

    Location_Setup(0);

    uint8_t i = 0;
    for (i = 0; i < 3; i++)
    {
        s_ADCs[i].begin();
        s_ADCs[i].setGain(GAIN_ONE);
    }

    FIELD_TYPE fieldTypes[] = {
        VOLTAGE,
        VOLTAGE,
        VOLTAGE,
        VOLTAGE,
        CURRENT,
        CURRENT,
        CURRENT,
        CURRENT,
        CURRENT,
        CURRENT,
        CURRENT,
        CURRENT,
        TEMPERATURE_C,
        TEMPERATURE_C,
        TEMPERATURE_C
    };
  
    int averaging_interval = Settings_getInt(DATA_AVERAGING_INTERVAL_SECS);
    int storage_interval = Settings_getInt(CSV_RECORD_INTERVAL_SECS);

    APP_SD_Init();
    APP_SD_Setup(30 * 1000);
    
    APP_SD_ReadSettings();

    APP_DATA_Setup(
        averaging_interval, // seconds to average readings over
        FIELD_COUNT, // Number of fields to create
        ADC_READS_PER_SECOND, // Number of reads per second per field
        storage_interval, // Number of seconds between SD card writes
        fieldTypes);

    Serial.print("ADC reads every ");
    Serial.print(MS_PER_ADC_READ);
    Serial.println("ms");

    Serial.print("Averaging every ");
    Serial.print(averaging_interval);
    Serial.println("s");

    pinMode(LED1_PIN, OUTPUT);
    pinMode(LED2_PIN, OUTPUT);

    readFromADCsTask.ResetTime();
}

void loop()
{
    readFromADCsTask.tick();
    APP_DATA_Tick();
    APP_SD_Tick();
    heartbeatTask.tick();
}

/*
 * APP_FatalError
 */
void APP_FatalError(char * err)
{
    Serial.println(err);

    while (1)
    {
        digitalWrite(LED2_PIN, HIGH);
        delay(200);
        digitalWrite(LED2_PIN, LOW);
        delay(200);
    }
}