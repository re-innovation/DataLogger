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
#include <LGSM.h>
#include <LGPS.h>
#include <LGPRS.h>
#include <LDateTime.h>
#include <LBattery.h>

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

#include "DLSMS.h"

#include "TaskAction.h"

/*
 * Application Includes
 */

#include "app.upload_manager.h"
#include "app.sd_storage.h"
#include "app.data.h"
#include "app.sms.h"

// Pointers to fuctionality objects

static ServiceInterface * s_thingSpeakService;
static NetworkInterface * s_gprsConnection;

static char * s_requestBuffer;
static uint32_t s_uploadBufferSize;
static float * s_uploadData;

static bool s_debugUpload = false;
static bool s_debugGPS = false;
static bool s_debugBattery = false;

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

static uint32_t s_uploadInterval = 0;

static TM s_gpsTime;
static TM s_rtcTime;

#define DATA_LED_PIN (5)
#define STATUS_LED_PIN (4)

#define ADC_READS_PER_SECOND (5)
#define MS_PER_ADC_READ (1000 / ADC_READS_PER_SECOND)

static int s_batteryWarnLevel = -1;

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
        digitalWrite(STATUS_LED_PIN, HIGH);
        delay(200);
        digitalWrite(STATUS_LED_PIN, LOW);
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
        Serial.println("Turning debugging on for local storage functionality.");
        APP_SD_EnableDebugging();
    }

    if (strstr(modules, "Upload"))
    {
        Serial.println("Turning debugging on for remote upload functionality.");
        s_debugUpload = true;
    }
    
    if (strstr(modules, "GPS"))
    {
        Serial.println("Turning debugging on for GPS functionality.");
        s_debugGPS = true;
    }

    if (strstr(modules, "Batt"))
    {
        Serial.println("Turning debugging on for battery functionality.");
        s_debugBattery = true;
    }
}

static void tryConnection(void)
{
    // Try to connect to GPRS network
    if (s_debugUpload)
    {
        Serial.print("Attempting to attach to ");
        Serial.print(Settings_getString(GPRS_APN));
        Serial.print(" with username/pwd ");
        Serial.print(Settings_getString(GPRS_USERNAME));
        Serial.print("/");
        Serial.println(Settings_getString(GPRS_PASSWORD));
    }
    s_gprsConnection->tryConnection(10);
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
    digitalWrite(STATUS_LED_PIN, ledState ? HIGH : LOW);
    ledState = !ledState;
}
TaskAction heartbeatTask(heartbeatTaskFn, 500, INFINITE_TICKS);

void batteryCheckTaskFn(void)
{
    int batteryLevel = LBattery.level();
    bool charging = LBattery.isCharging();

    if (s_debugBattery)
    {
        Serial.print("Battery level: ");
        Serial.print(batteryLevel);
        Serial.println('%');
    }

    if (s_batteryWarnLevel <= 0) { return; }

    char sms[140];

    if (batteryLevel <= s_batteryWarnLevel)
    {
        if (charging)
        {
            sprintf(sms, "Low Battery Warning: %d%% (charging)", batteryLevel);
        }
        else
        {
            sprintf(sms, "Low Battery Warning: %d%% (not charging)", batteryLevel);
        }
        APP_SMS_SendMaintenanceMessage(sms);
    }
}
TaskAction batteryCheckTask(batteryCheckTaskFn, 60*60*1000, INFINITE_TICKS);

void readFromADCsTaskFn(void)
{
    uint8_t adc = 0;
    uint8_t ch = 0;
    uint8_t field = 0;
    int32_t allData[15];

    // Read the ADC1x1x ICs for fields 1 - 12
    for (adc = 0; adc < 3; adc++)
    {
        for (ch = 0; ch < 4; ch++)
        {
            field = (adc*4)+ch;
            if (Settings_ChannelSettingIsValid(field+1))
            {
                allData[field] = s_ADCs[adc].readADC_SingleEnded(ch);
            }
            else
            {
                allData[field] = 0;
            }
        }
    }

    // Read the internal ADCs for fields 13-15
    for (adc = 0; adc < 3; adc++)
    {
        field = adc + 13;
        if (Settings_ChannelSettingIsValid(field+1))
        {
            allData[field] = s_internalADCs[adc].read();
        }
        else
        {
            allData[field] = 0;
        }
    }
    
    APP_DATA_NewDataArray(allData);
}
TaskAction readFromADCsTask(readFromADCsTaskFn, MS_PER_ADC_READ, INFINITE_TICKS, "ADC Read Task");

TaskAction remoteUploadTask(remoteUploadTaskFn, 1000, INFINITE_TICKS, "Upload Task");
void remoteUploadTaskFn(void)
{
    if (APP_DATA_UploadIsPending() && APP_DATA_UploadDataRemaining())
    {
        /* The data upload flag is set and there is data to upload.
        Reset the flag and task interval and perform the upload */
        APP_DATA_SetUploadPending(false);
        remoteUploadTask.SetInterval(s_uploadInterval * 1000);

        digitalWrite(DATA_LED_PIN, HIGH);
        uint16_t nFields = APP_DATA_GetNumberOfFields();
        
        if (!s_gprsConnection->isConnected())
        {
            if (s_debugUpload) { Serial.println("GPRS not connected. Attempting new connection."); }
            tryConnection();
        }

        // Try to upload to ThingSpeak
        char response_buffer[200] = "";
        while(APP_DATA_UploadDataRemaining())
        {

            Serial.print("Remaining records to upload: ");
            Serial.println(APP_DATA_UploadDataRemaining());

            if (s_debugUpload)
            {
                Serial.print("Attempting upload to ");
                Serial.println(s_thingSpeakService->getURL());
            }
            
            APP_DATA_GetUploadData(s_uploadData);
            
            char created_at[30];
            TM createTime;
            Time_GetTime(&createTime, TIME_PLATFORM);
            CSV_writeTimestampToBuffer(&createTime, created_at);
            s_thingSpeakService->createPostAPICall(
                s_requestBuffer, s_uploadData, APP_DATA_GetChannelNumbers(), nFields, s_uploadBufferSize, NULL);

            if (s_debugUpload)
            {
                Serial.print("Request: '");
                Serial.print(s_requestBuffer);
                Serial.println("'");
            }

            if (s_gprsConnection->sendHTTPRequest(s_thingSpeakService->getURL(), s_requestBuffer, response_buffer))
            {
                if (s_debugUpload) { Serial.println("Upload complete!"); }
            }
            else
            {
                if (s_debugUpload)
                {
                    Serial.print("Could not connect to ");
                    Serial.print(s_thingSpeakService->getURL());
                    Serial.println("!");
                }
            }
        }
        
        digitalWrite(DATA_LED_PIN, LOW);
    }
    else
    {
        /* Set the upload pending flag and start calling this task every second to
        check for data */
        APP_DATA_SetUploadPending(true);
        remoteUploadTask.SetInterval(1000);
    }
}

void gpsTaskFn(void)
{
    Location_UpdateNow();
    bool success = GPS_InfoIsValid();

    if (success)
    {
        if (s_debugGPS) { Serial.println("Updating RTC time from GPS."); }
        Time_GetTime(&s_gpsTime, TIME_GPS);
        Time_SetPlatformTime(&s_gpsTime);
    }
    else
    {
        if (s_debugGPS) { Serial.println("No valid GPS info."); }
    }
}
TaskAction gpsTask(gpsTaskFn, 30 * 1000, INFINITE_TICKS, "GPS Task");

void setupUploadVars(void)
{
    uint16_t nFields = APP_DATA_GetNumberOfFields();

    /* Get pointers to the GPRS data and online storage services */
    s_thingSpeakService = Service_GetService(SERVICE_THINGSPEAK);
    s_gprsConnection = Network_GetNetwork(NETWORK_INTERFACE_LINKITONE_GPRS);

    // Allocate space for CSV data and HTTP request building.
    // Allow 10 chars per field, plus 20 for timestamp.
    // Then allocate twice as much as that estimate (minimum 512)

    s_uploadBufferSize = APP_DATA_GetUploadBufferSize();
    Serial.print("Upload buffer size: ");
    Serial.println(s_uploadBufferSize);
    
    s_requestBuffer = new char [s_uploadBufferSize];

    // Allocate space for floats to pass to upload module
    s_uploadData = new float[nFields];
}

void setupADCs(void)
{
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
}

void setupTime(void)
{
    Time_GetTime(&s_rtcTime, TIME_PLATFORM);
    Serial.print("RTC datetime: ");
    Time_PrintTime(&s_rtcTime);
    Serial.print(" ");
    Time_PrintDate(&s_rtcTime, true);
}

void setupBattery(void)
{
    if (Settings_intIsSet(BATTERY_WARN_LEVEL))
    {
        s_batteryWarnLevel = Settings_getInt(BATTERY_WARN_LEVEL);
        unsigned long intervalInMinutes = 60;  // Default to 1 hour
        
        if (Settings_intIsSet(BATTERY_WARN_INTERVAL_MINUTES))
        {
            unsigned long intervalInMinutes = Settings_getInt(BATTERY_WARN_INTERVAL_MINUTES);
        }    
        
        batteryCheckTask.SetInterval(intervalInMinutes * 60 * 1000);   
        
        Serial.print("Setting battery warning to ");
        Serial.print(s_batteryWarnLevel);
        Serial.print(" and checking every ");
        Serial.print(intervalInMinutes);
        Serial.println(" minutes.");

        // Call the check task right away in case the battery is below level now
        batteryCheckTaskFn();
    }
    else
    {
        s_batteryWarnLevel = 101; // Set 101% so warning is never issued
    }
}

void setup()
{   
    // setup Serial port
    Serial.begin(115200);

    delayStart(10);

    pinMode(DATA_LED_PIN, OUTPUT);
    pinMode(STATUS_LED_PIN, OUTPUT);

    Location_Setup(0);
 
    APP_SD_Init(); // This just initialises the SD card module so settings can be read
    
    /* Tell the settings modules which settings are 
    required. If these settings are missing, the application
    will not start (APP_FatalError will be called) */
    Settings_requireInt(UPLOAD_AVERAGING_INTERVAL_SECS);
    Settings_requireInt(STORAGE_AVERAGING_INTERVAL_SECS);
    Settings_requireInt(DATA_STORAGE_INTERVAL_SECS);
    Settings_requireInt(DATA_UPLOAD_INTERVAL_SECS);
    
    Settings_requireString(GPRS_APN);
    Settings_requireString(GPRS_USERNAME);

    Settings_requireString(THINGSPEAK_URL);
    Settings_requireString(THINGSPEAK_API_KEY);
    
    APP_SD_ReadGlobalSettings("Datalogger.settings.conf");
    
    /* If exectution proceeds this far, all required settings were read.
    Start configuring the application */
    int storage_averaging_interval = Settings_getInt(STORAGE_AVERAGING_INTERVAL_SECS);
    int upload_averaging_interval = Settings_getInt(UPLOAD_AVERAGING_INTERVAL_SECS);
    int storage_interval = Settings_getInt(DATA_STORAGE_INTERVAL_SECS);
    s_uploadInterval = Settings_getInt(DATA_UPLOAD_INTERVAL_SECS);

    // The SD card module needs to know how often writes occur
    APP_SD_Setup(storage_interval * 1000);

    APP_DATA_Setup(
        storage_averaging_interval, // Seconds to average storage readings over
        upload_averaging_interval,  // Seconds to average upload readings over
        ADC_READS_PER_SECOND, // Number of reads per second per field
        storage_interval, // Number of seconds between SD card writes
        s_uploadInterval, // Number of seconds between uploads to thingspeak
        "Datalogger.channels.conf" // Filename to read channel settings from
        );

    /* If execution got this far, the application data storage functionality
    was set up successfully and there are valid fields.
    Configure the remaining application functionality. */

    APP_SMS_Setup();

    setupBattery();

    setupTime();

    setupUploadVars();
    
    setupADCs();
    
    readFromADCsTask.ResetTime();

    remoteUploadTask.SetInterval(s_uploadInterval * 1000);
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
    batteryCheckTask.tick();
}

