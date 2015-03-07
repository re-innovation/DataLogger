/*
 * LinkIT ONE
 *
 * DataLogger Application on the LinkItONE platform.
 *
 * See README.md for full details
 *
 * Basic summary:
 *
 * - Reads 2x voltages and 4x currents from i2c sensors
 * - Performs averaging of data
 * - Stores as CSV files on uSD card
 * - Sends data to Thingspeak service over GPRS
 * - Uses GPS for position testing and RTC updates
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
#include <LSD.h>
#include <LGPS.h>
#include <LStorage.h>

/*
 * DataLogger Includes
 */

#include "DLUtility.h"
#include "DLSettings.h"
#include "DLLocation.h"
#include "DLService.h"
#include "DLSecurity.h"
#include "DLHTTP.h"
#include "DLNetwork.h"
#include "DLDataField.h"
#include "DLService.ThingSpeak.h"
#include "DLLocalStorage.h"
#include "DLSensor.ADS1x1x.h"
#include "TaskAction.h"

// Pointers to fuctionality objects
static ServiceInterface * s_thingSpeakService;
static NetworkInterface * s_gprsConnection;
static LocalStorageInterface * s_sdCard;
static ADS1115 s_ADC();

static LocationAlarm s_locationAlarm(onAlarmTripped, 50, NULL);
static LOCATION_2D s_currentLocation;

// In-RAM data storage
static DataField s_dataFields[] = {
    DataField(VOLTAGE),
    DataField(VOLTAGE),
    DataField(CURRENT),
    DataField(CURRENT),
    DataField(CURRENT),
    DataField(CURRENT)
};

// Tasks
TaskAction remoteUploadTask(remoteUploadTaskFn, MS_PER_HOUR * 2, INFINITE_TICKS);
void remoteUploadTaskFn(void)
{
    if (!s_gprsConnection->isConnected())
    {
        Serial.println("GPRS not connected. Attempting new connection.");
        tryConnection();
    }

    // Try to upload to ThingSpeak
    Serial.print("Attempting upload to ");
    Serial.println(s_thingSpeakService->getURL());

    char request_buffer[300];
    char response_buffer[200] = "";
    s_thingSpeakService->createPostAPICall(request_buffer, 300);

    Serial.print("Request '");
    Serial.print(request_buffer);
    Serial.println("'");

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
}

TaskAction gpsCheckTask(gpsCheckTaskFn, 10000, INFINITE_TICKS);
void gpsCheckTaskFn(void)
{
    float offset = (float)random(-100,100); // Random offset in meters

    Serial.print("Simulating location change of ");
    Serial.print(offset);
    Serial.println("m");

    // Convert offset to decimal degrees
    offset *= 360.0f;
    offset /= MEAN_EARTH_CIRCUMFERENCE_M;

    Location_UpdateNow();
    if ( Location_GetLocation_2D(&s_currentLocation) )
    {
        s_currentLocation.latitude += offset;
        s_locationAlarm.update(&s_currentLocation);
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


static void createFakeData(void)
{
    float randomData = 0.0;
    uint8_t f = 0;

    for (f = 0; f < 6; f++)
    {
        if (f < 2)
        {
            // First two fields are random voltage between 40V and 60V
            randomData = (float)random(4000, 6000)/100.0;
            s_dataFields[f].StoreData(randomData);
        }
        else
        {
            // Other fields are random current between 0 and 10A
            randomData = (float)random(0, 1000)/100.0;
        }

        s_dataFields[f].StoreData(randomData);
        s_thingSpeakService->setField(f, &s_dataFields[f]);
    }
}

void onAlarmTripped(void)
{
    Serial.println("Location alarm tripped!");
}

void setup()
{
    // setup Serial port
    Serial.begin(115200);

    delay(10000);

    Settings_setString(GPRS_APN, "everywhere");
    Settings_setString(GPRS_USERNAME, "eesecure");
    Settings_setString(GPRS_PASSWORD, "secure");

    // Thingspeak URL choices (for development)
    Settings_setString(THINGSPEAK_URL, "api.thingspeak.com"); // Thingspeak hosted services
    //Settings_setString(THINGSPEAK_URL, "agile-headland-8076.herokuapp.com"); // Mouse's heroku instance

    // Thingspeak API keys
    Settings_setString(THINGSPEAK_API_KEY, "NNMOIJ91NG0MRNNX"); // My Thingspeak API key
    //Settings_setString(THINGSPEAK_API_KEY, "0CKUYB7VJI2I9WG9"); // Matt Little's API key
    //Settings_setString(THINGSPEAK_API_KEY, "IZ2O45C3BM257VCH"); // Mouse's API key
    
    s_thingSpeakService = Service_GetService(SERVICE_THINGSPEAK);
    s_gprsConnection = Network_GetNetwork(NETWORK_INTERFACE_LINKITONE_GPRS);
    s_sdCard = LocalStorage_GetLocalStorageInterface(LINKITONE_SD_CARD);

    Location_Setup(0);
    Location_UpdateNow();
    Location_GetLocation_2D(&s_currentLocation);
    s_locationAlarm.setHome(&s_currentLocation);

}

void loop()
{
    //createFakeData();

    //remoteUploadTaskFn();

    gpsCheckTask.tick();
}
