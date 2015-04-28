/*
 * LinkItONE.Crest
 *
 * Application for Crest PV on LinkItONE hardware
 *
 * See README.md for full details
 *
 * Basic summary:
 *
 * - Reads data from ADS1115 ADCs (addresses 0x48, 0x49, 0x4A) 10x per second
 * - Averages data
 * - Stores data to SD card
 * - Sends data to Thingspeak
 * - configuration of these functions through files on SD card
 *
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

static char * s_requestBuffer;
static uint32_t s_uploadBufferSize;
static float * s_uploadData;

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

static void updateUploadData(uint16_t dataIndex)
{
    uint16_t i;
    uint16_t nFields = APP_DATA_GetNumberOfFields();
    for(i = 0; i < nFields; ++i)
    {
        s_uploadData[i] = APP_Data_GetUploadField(i)->getConvData(dataIndex);
    }
}

static void delayStart(uint8_t seconds)
{
    Serial.println("Starting application in... ");

    while (seconds--)
    {
        Serial.print(seconds);
        Serial.print("...");
        delay(1000);
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
                APP_DATA_NewData(data, field);
            }
        }
    }
    // Count how much new data is stored
    APP_DATA_IncrementCounts();
}

TaskAction readFromADCsTask(readFromADCsTaskFn, MS_PER_ADC_READ, INFINITE_TICKS);

TaskAction remoteUploadTask(remoteUploadTaskFn, 0, INFINITE_TICKS);
void remoteUploadTaskFn(void)
{

    digitalWrite(LED1_PIN, HIGH);
    
    uint16_t nFields = APP_DATA_GetNumberOfFields();
    
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

    uint16_t toUpload = APP_DATA_GetToUploadCount();
    int32_t i;
    for (i = toUpload-1; i >= 0; ++i)
    {    
        updateUploadData(i);
        
        char created_at[30];
        TM createTime;
        Time_GetTime(&createTime, TIME_PLATFORM);
        CSV_writeTimestampToBuffer(&createTime, created_at);
        s_thingSpeakService->createPostAPICall(
            s_requestBuffer, s_uploadData, nFields, s_uploadBufferSize, NULL);

        if (s_gprsConnection->sendHTTPRequest(s_thingSpeakService->getURL(), s_requestBuffer, response_buffer))
        {
            Serial.println(" upload complete!");
        }
        else
        {
            Serial.print(" could not connect to ");
            Serial.print(s_thingSpeakService->getURL());
            Serial.println("!");
        }
    }
    
    APP_DATA_ResetUploadCount();

    digitalWrite(LED1_PIN, LOW);
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

    delayStart(10);

    Location_Setup(0);
 
    APP_SD_Init();
    
    Settings_requireInt(DATA_AVERAGING_INTERVAL_SECS);
    Settings_requireInt(DATA_STORAGE_INTERVAL_SECS);
    Settings_requireInt(DATA_UPLOAD_INTERVAL_SECS);
    
    Settings_requireString(GPRS_APN);
    Settings_requireString(GPRS_USERNAME);
    Settings_requireString(GPRS_PASSWORD);
    Settings_requireString(THINGSPEAK_URL);
    Settings_requireString(THINGSPEAK_API_KEY);
    
    APP_SD_ReadGlobalSettings("Datalogger.settings.conf");
    
    int averaging_interval = Settings_getInt(DATA_AVERAGING_INTERVAL_SECS);
    int storage_interval = Settings_getInt(DATA_STORAGE_INTERVAL_SECS);
    int upload_interval = Settings_getInt(DATA_UPLOAD_INTERVAL_SECS);

    APP_SD_Setup(storage_interval * 1000);

    APP_DATA_Setup(
        averaging_interval, // Seconds to average readings over
        ADC_READS_PER_SECOND, // Number of reads per second per field
        storage_interval, // Number of seconds between SD card writes
        upload_interval, // Number of seconds between uploads to thingspeak
        "Datalogger.channels.conf" // Filename to read channel settings from
        );

    s_thingSpeakService = Service_GetService(SERVICE_THINGSPEAK);
    s_gprsConnection = Network_GetNetwork(NETWORK_INTERFACE_LINKITONE_GPRS);

    uint16_t nFields = APP_DATA_GetNumberOfFields();
    
    // Allocate space for CSV data and HTTP request building.
    // Allow 10 chars per field, plus 20 for timestamp.
    // Then allocate twice as much as that estimate.

    s_uploadBufferSize = ((10 * nFields) + 20) * APP_DATA_GetNumberOfAveragesForUpload() * 2;
    s_requestBuffer = new char [s_uploadBufferSize];

    // Allocate space for floats to pass to upload module
    s_uploadData = new float[nFields];

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
    remoteUploadTask.SetInterval(upload_interval * 1000);
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
