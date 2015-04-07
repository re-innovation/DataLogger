/*
 * LinkItONE.Crest
 *
 * Application for Crest PV on LinkItONE hardware
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
#include "DLHTTP.h"
#include "DLNetwork.h"
#include "DLService.h"
#include "DLService.thingspeak.h"

#include "TaskAction.h"

/*
 * Application Includes
 */

#include "app.upload_manager.h"
#include "app.sd_storage.h"
#include "app.data.h"

// Pointers to fuctionality objects

static ServiceInterface * s_thingSpeakService;
static NetworkInterface * s_gprsConnection;

static ADS1115 s_ADCs[] = {
    ADS1115(0x48),
    ADS1115(0x49),
    ADS1115(0x4A)
};

static LinkItONEADC s_adcs[] = {
    LinkItONEADC(A0),
    LinkItONEADC(A1),
    LinkItONEADC(A2)
};

static Thermistor s_thermistors[] = {
    Thermistor(3977, 10000),
    Thermistor(3977, 10000),
    Thermistor(3977, 10000)
};

// There are 12 averages for voltage and current data
#define V_AND_I_COUNT (12)

// ...plus three more fields for temperature data
#define THERMISTOR_COUNT (3)

#define FIELD_COUNT (V_AND_I_COUNT + THERMISTOR_COUNT)

#define ADC_READS_PER_SECOND (10)

#define MS_PER_ADC_READ (1000 / ADC_READS_PER_SECOND)

static uint32_t s_uploadBufferSize;
static char * s_csvData;
static char * s_requestBuffer;

static char s_settingsFilename[] = "Datalogger.Settings";

static void tryConnection(void)
{
    // Try to connect to GPRS network
    Serial.print("Attempting to attach to ");
    Serial.print(Settings_getString(GPRS_APN));
    Serial.print(" with username/pwd ");
    Serial.print(Settings_getString(GPRS_USERNAME));
    Serial.print("/");
    Serial.println(Settings_getString(GPRS_PASSWORD));
    
    s_gprsConnection->tryConnection(10);
}

static uint32_t s_storeToSDIntervalms;

/*
 * Tasks
 */
TaskAction remoteUploadTask(remoteUploadTaskFn, 0, INFINITE_TICKS);
void remoteUploadTaskFn(void)
{

    if (!s_gprsConnection->isConnected())
    {
        Serial.println("GPRS not connected. Attempting new connection.");
        tryConnection();
    }

    // Try to upload to ThingSpeak
    Serial.print("Attempting bulk upload to ");
    Serial.println(s_thingSpeakService->getURL());

    char response_buffer[200] = "";

    APP_SD_ReadAllDataFromCurrentFile(s_csvData, s_uploadBufferSize);

    s_thingSpeakService->createBulkUploadCall(
        s_requestBuffer, s_uploadBufferSize, s_csvData, Filename_get(), 15);

    /*Serial.print("Request '");
    Serial.print(s_requestBuffer);
    Serial.println("'");*/

    if (s_gprsConnection->sendHTTPRequest(s_thingSpeakService->getURL(), s_requestBuffer, response_buffer))
    {
        Serial.print("Got response (");   
        Serial.print(strlen(response_buffer));
        Serial.print(" bytes):");   
        Serial.println(response_buffer);
    }
    else
    {
        Serial.print("Could not connect to ");
        Serial.println(s_thingSpeakService->getURL());
    }

    // Start a new file after upload
    APP_SD_CreateNewDataFile();
}

TaskAction readFromADCsTask(readFromADCsTaskFn, MS_PER_ADC_READ, INFINITE_TICKS);
void readFromADCsTaskFn(void)
{
    uint8_t i;
    uint8_t adc = 0;
    uint8_t ch = 0;
    uint8_t field = 0;

    // Read the ADC1x1x ICs for fields 1 - 12
    for (adc = 0; adc < 3; adc++)
    {
        for (ch = 0; ch < 4; ch++)
        {
            field = (adc*4)+ch;
            float data = s_ADCs[adc].readADC_SingleEnded(ch);
            APP_DATA_NewData(data, field);
        }
    }

    // Then read the three thermistors
    // The ADC range is 0 to 5v, but thermistors are on 3.3v rail, so maximum is 1023 * 3.3/5 = 675
    for (i = 0; i < THERMISTOR_COUNT; i++)
    {
        float data = s_thermistors[i].TemperatureFromADCReading(10000.0, s_adcs[i].read(), 675);
        APP_DATA_NewData(data, V_AND_I_COUNT + i);
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

    APP_SD_Init();

    APP_SD_ReadSettings(s_settingsFilename);
    
    int averaging_interval = Settings_getInt(DATA_AVERAGING_INTERVAL_SECS);
    int uploadInterval = Settings_getInt(THINGSPEAK_UPLOAD_INTERVAL);
    int averagesToStore = uploadInterval/averaging_interval;

    APP_SD_DataSetup(uploadInterval * 1000, averagesToStore);

    APP_DATA_Setup(averaging_interval * 1000,
        FIELD_COUNT, ADC_READS_PER_SECOND * averaging_interval, averagesToStore, fieldTypes);

    APP_SD_CreateNewDataFile();
    
    s_thingSpeakService = Service_GetService(SERVICE_THINGSPEAK);
    s_gprsConnection = Network_GetNetwork(NETWORK_INTERFACE_LINKITONE_GPRS);

    // Allocate space for CSV data and HTTP request building.
    // Allow 10 chars per field, plus 20 for timestamp.
    // Then allocate twice as much as that estimate.

    s_uploadBufferSize = ((10 * FIELD_COUNT) + 20) * averagesToStore * 2;
    s_csvData = new char [s_uploadBufferSize];
    s_requestBuffer = new char [s_uploadBufferSize];

    Serial.print("ADC reads every ");
    Serial.print(MS_PER_ADC_READ);
    Serial.println("ms");

    Serial.print("Averaging every ");
    Serial.print(averaging_interval);
    Serial.println("s");

    Serial.print("Storing to SD and uploading every ");
    Serial.print(uploadInterval);
    Serial.println("s");

    Serial.print("Created buffers of size ");
    Serial.print(s_uploadBufferSize);
    Serial.println("B");

    Serial.print("Setting RTC...");
    TM buildTime;
    buildTime.tm_year = GREGORIAN_TO_C_YEAR(2015);
    buildTime.tm_mon = 4;
    buildTime.tm_mday = 7;
    buildTime.tm_hour = 1;
    buildTime.tm_min = 20;
    buildTime.tm_sec = 0;
    
    Time_SetPlatformTime(&buildTime);
    
    readFromADCsTask.ResetTime();
    remoteUploadTask.SetInterval(uploadInterval * 1000);
    remoteUploadTask.ResetTime();
}

void loop()
{
    readFromADCsTask.tick();
    APP_DATA_Tick();
    APP_SD_Tick();
    remoteUploadTask.tick();
}
