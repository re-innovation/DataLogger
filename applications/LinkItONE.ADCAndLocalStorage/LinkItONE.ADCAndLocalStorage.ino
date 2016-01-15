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
#include "DLDataField.Types.h"
#include "DLDataField.h"
#include "DLDataField.Manager.h"
#include "DLSettings.h"
#include "DLSettings.Global.h"
#include "DLSettings.Reader.Errors.h"
#include "DLSettings.Global.Reader.h"
#include "DLSettings.Reader.h"
#include "DLSettings.DataChannels.h"
#include "DLSensor.ADS1x1x.h"
#include "DLSensor.LinkItONE.h"
#include "DLSensor.Thermistor.h"
#include "DLPlatform.h"

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

#define ADC_READS_PER_SECOND (10)
#define MS_PER_ADC_READ (1000 / ADC_READS_PER_SECOND)

/*
 * APP_FatalError
 *
 * Prints error to serial and then loops flashing LED
 */
void APP_FatalError(char const * const err)
{
    Serial.println("Application error:");
    Serial.println(err);

    #ifdef ARDUINO
    while (1)
    {
        digitalWrite(LED2_PIN, HIGH);
        delay(200);
        digitalWrite(LED2_PIN, LOW);
        delay(200);
    }
    #else
    _exitMock();
    #endif    
}

/*
 * APP_SetDebugModules
 *
 * For each module in the comma separated list of modules, turn serial debugging on
 */
void APP_SetDebugModules(char const * const modules)
{
    if (strstr(modules, "LocalStorage"))
    {
        Serial.println("Turning debugging on for LocalStorage module.");
        APP_SD_EnableDebugging();
    }
}

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
            if (Settings_ChannelSettingIsValid(field))
            {
                uint16_t data = s_ADCs[adc].readADC_SingleEnded(ch);
                APP_Data_NewData(data, field);
            }
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
 
    APP_SD_Init();
    APP_SD_Setup(30 * 1000);
    
    Settings_requireInt(STORAGE_AVERAGING_INTERVAL_SECS);
    Settings_requireInt(DATA_STORAGE_INTERVAL_SECS);

    APP_SD_ReadGlobalSettings("Datalogger.settings.conf");
    
    int averaging_interval = Settings_getInt(STORAGE_AVERAGING_INTERVAL_SECS);
    int storage_interval = Settings_getInt(DATA_STORAGE_INTERVAL_SECS);

    APP_Data_Setup(
        averaging_interval, // Seconds to average readings over
        ADC_READS_PER_SECOND, // Number of reads per second per field
        storage_interval, // Number of seconds between SD card writes
        "Datalogger.channels.conf" // Filename to read channel settings from
        );

    pinMode(LED1_PIN, OUTPUT);
    pinMode(LED2_PIN, OUTPUT);

    uint8_t i = 0;
    for (i = 0; i < 3; i++)
    {
        s_ADCs[i].begin();
        s_ADCs[i].setGain(GAIN_ONE);

        if (Settings_stringIsSet(FAKE_ADC_READS))
        {
            s_ADCs[i].fake(0, 0, 1023);
            s_ADCs[i].fake(1, 0, 1023);
            s_ADCs[i].fake(2, 0, 1023);
            s_ADCs[i].fake(3, 0, 1023);
        }
    }

    readFromADCsTask.ResetTime();
}

void loop()
{
    readFromADCsTask.tick();
    APP_Data_Tick();
    APP_SD_Tick();
    heartbeatTask.tick();
}
