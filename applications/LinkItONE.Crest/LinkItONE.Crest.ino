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

static LinkItONEADC s_internalADCs[] = {
    LinkItONEADC(A0),
    LinkItONEADC(A1),
    LinkItONEADC(A2)
};

#define LED1_PIN (4)
#define LED2_PIN (5)

// There are 12 averages for voltage and current data
#define V_AND_I_COUNT (12)

// ...plus three more fields for temperature data
#define THERMISTOR_COUNT (3)

#define FIELD_COUNT (V_AND_I_COUNT + THERMISTOR_COUNT)

#define ADC_READS_PER_SECOND (10)
#define AVERAGING_PERIOD_SECONDS (30)

#define MS_PER_ADC_READ (1000 / ADC_READS_PER_SECOND)

#define BULK_UPLOAD_BUFFER_SIZE (32768)

// Medium term in-RAM data storage and remote storage
#define STORE_TO_SD_EVERY_N_AVERAGES (1)
#define STORE_TO_SD_INTERVAL_MS (STORE_TO_SD_EVERY_N_AVERAGES * AVERAGING_PERIOD_SECONDS * 1000)
#define UPLOAD_EVERY_N_AVERAGES (1)
#define UPLOAD_INTERVAL_MS (UPLOAD_EVERY_N_AVERAGES * AVERAGING_PERIOD_SECONDS * 1000)

static char * csvData;
static char * requestBuffer;

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

TaskAction heartbeatTask(heartbeatTaskFn, 500, INFINITE_TICKS);
void heartbeatTaskFn(void)
{
    static bool ledState = false;
    digitalWrite(LED2_PIN, ledState ? HIGH : LOW);
    ledState = !ledState;
}

TaskAction remoteUploadTask(remoteUploadTaskFn, UPLOAD_INTERVAL_MS, INFINITE_TICKS);
void remoteUploadTaskFn(void)
{

    digitalWrite(LED1_PIN, HIGH);
    
    if (!s_gprsConnection->isConnected())
    {
        Serial.println("GPRS not connected. Attempting new connection.");
        tryConnection();
    }

    // Try to upload to ThingSpeak
    Serial.print("Attempting upload to ");
    Serial.print(s_thingSpeakService->getURL());
    Serial.print("...");
    char response_buffer[200] = "";

    float data[FIELD_COUNT];
    NumericDataField<float> ** dataFields = APP_DATA_GetDataFieldsPtr();
    uint8_t i;
    for(i = 0; i < FIELD_COUNT; ++i)
    {
        data[i] = dataFields[i]->getData(0);
    }

    char created_at[30];
    TM createTime;
    Time_GetTime(&createTime, TIME_PLATFORM);
    CSV_writeTimestampToBuffer(&createTime, created_at);
    s_thingSpeakService->createPostAPICall(requestBuffer, data, FIELD_COUNT, BULK_UPLOAD_BUFFER_SIZE, NULL);

    /*Serial.println("Upload request created:");
    Serial.println(requestBuffer);
    Serial.print("(");
    Serial.print(strlen(requestBuffer));
    Serial.println(") bytes.");*/

    if (s_gprsConnection->sendHTTPRequest(s_thingSpeakService->getURL(), requestBuffer, response_buffer))
    {
        /*Serial.print("Got response (");   
        Serial.print(strlen(response_buffer));
        Serial.print(" bytes):");   
        Serial.println(response_buffer);*/
        Serial.println(" upload complete!");
    }
    else
    {
        Serial.print(" could not connect to ");
        Serial.print(s_thingSpeakService->getURL());
        Serial.println("!");
    }

    digitalWrite(LED1_PIN, LOW);
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
            uint16_t data = s_ADCs[adc].readADC_SingleEnded(ch);
            APP_DATA_NewData(data, field);
        }
    }

    // Then read the three thermistors
    // The ADC range is 0 to 5v, but thermistors are on 3.3v rail, so maximum is 1023 * 3.3/5 = 675
    for (i = 0; i < THERMISTOR_COUNT; i++)
    {
        uint16_t data = s_internalADCs[i].read();
        APP_DATA_NewData(data, V_AND_I_COUNT + i);
    }
}

TaskAction gpsTask(gpsTaskFn, 30 * 1000, INFINITE_TICKS);
void gpsTaskFn(void)
{
    Location_UpdateNow();
    bool success = GPS_InfoIsValid();

    if (success)
    {
        Serial.println("Updating RTC time from GPS.");
        TM gpsTime;
        Time_GetTime(&gpsTime, TIME_GPS);
        Time_SetPlatformTime(&gpsTime);
    }
    else
    {
        Serial.println("No valid GPS info.");
    }
}

void setup()
{
    // setup Serial port
    Serial.begin(115200);

    delay(10000);

    Location_Setup(0);

    csvData = new char[BULK_UPLOAD_BUFFER_SIZE];
    requestBuffer = new char[BULK_UPLOAD_BUFFER_SIZE];

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

    APP_SD_Init();
    APP_SD_Setup(30 * 1000);
    APP_SD_ReadSettings();

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

    Serial.print("Uploading reads every ");
    Serial.print(UPLOAD_INTERVAL_MS/1000);
    Serial.println("s");

    pinMode(LED1_PIN, OUTPUT);
    pinMode(LED2_PIN, OUTPUT);

    readFromADCsTask.ResetTime();
    remoteUploadTask.ResetTime();
    gpsTask.ResetTime();
}

void loop()
{
    readFromADCsTask.tick();
    APP_DATA_Tick();
    APP_SD_Tick();
    remoteUploadTask.tick();
    heartbeatTask.tick();
    gpsTask.tick();
}