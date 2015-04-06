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
#define AVERAGING_PERIOD_SECONDS (1)

#define MS_PER_ADC_READ (1000 / ADC_READS_PER_SECOND)

#define BULK_UPLOAD_BUFFER_SIZE (16384)

// Medium term in-RAM data storage and remote storage
#define STORE_TO_SD_EVERY_N_AVERAGES (10)
#define STORE_TO_SD_INTERVAL_MS (STORE_TO_SD_EVERY_N_AVERAGES * AVERAGING_PERIOD_SECONDS * 1000)
#define UPLOAD_EVERY_N_AVERAGES (10)
#define UPLOAD_INTERVAL_MS (UPLOAD_EVERY_N_AVERAGES * AVERAGING_PERIOD_SECONDS * 1000)

static char csvData[BULK_UPLOAD_BUFFER_SIZE];
static char request_buffer[BULK_UPLOAD_BUFFER_SIZE];


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

/*
 * Tasks
 */
TaskAction remoteUploadTask(remoteUploadTaskFn, UPLOAD_INTERVAL_MS, INFINITE_TICKS);
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

    APP_SD_ReadAllDataFromCurrentFile(csvData, BULK_UPLOAD_BUFFER_SIZE);

    s_thingSpeakService->createBulkUploadCall(
        request_buffer, BULK_UPLOAD_BUFFER_SIZE, csvData, Filename_get(), 15);

    /*Serial.print("Request '");
    Serial.print(request_buffer);
    Serial.println("'");*/

    if (s_gprsConnection->sendHTTPRequest(s_thingSpeakService->getURL(), request_buffer, response_buffer))
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

    APP_DATA_Setup(AVERAGING_PERIOD_SECONDS * 1000,
        FIELD_COUNT, ADC_READS_PER_SECOND * AVERAGING_PERIOD_SECONDS, STORE_TO_SD_EVERY_N_AVERAGES, fieldTypes);

    APP_SD_Setup(STORE_TO_SD_INTERVAL_MS, STORE_TO_SD_EVERY_N_AVERAGES);
    APP_SD_CreateNewDataFile();

    Settings_setString(GPRS_APN, "everywhere");
    Settings_setString(GPRS_USERNAME, "eesecure");
    Settings_setString(GPRS_PASSWORD, "secure");

    // Thingspeak URL choices (for development)
    Settings_setString(THINGSPEAK_URL, "agile-headland-8076.herokuapp.com"); // Thingspeak hosted services

    // Thingspeak API keys
    Settings_setString(THINGSPEAK_API_KEY, "IZ2O45C3BM257VCH"); // Mouse's API key
    
    s_thingSpeakService = Service_GetService(SERVICE_THINGSPEAK);
    s_gprsConnection = Network_GetNetwork(NETWORK_INTERFACE_LINKITONE_GPRS);

    Serial.print("ADC reads every ");
    Serial.print(MS_PER_ADC_READ);
    Serial.println("ms");

    Serial.print("Averaging every ");
    Serial.print(AVERAGING_PERIOD_SECONDS);
    Serial.println("s");

    Serial.print("Storing to SD every ");
    Serial.print(STORE_TO_SD_INTERVAL_MS/1000);
    Serial.println("s");

    Serial.print("Uploading reads every ");
    Serial.print(UPLOAD_INTERVAL_MS/1000);
    Serial.println("s");

    Serial.print("Setting RTC...");
    TM buildTime;
    buildTime.tm_year = GREGORIAN_TO_C_YEAR(2015);
    buildTime.tm_mon = 4;
    buildTime.tm_mday = 6;
    buildTime.tm_hour = 23;
    buildTime.tm_min = 5;
    buildTime.tm_sec = 0;
    
    Time_SetPlatformTime(&buildTime);
    
    readFromADCsTask.ResetTicks();
    remoteUploadTask.ResetTicks();
}

void loop()
{
    readFromADCsTask.tick();
    APP_DATA_Tick();
    APP_SD_Tick();
    remoteUploadTask.tick();
}