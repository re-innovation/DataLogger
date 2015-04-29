/*
 * LinkItONE.ADCAndThingspeak
 *
 * Example application for the LinkIt ONE
 *
 * See README.md for full details
 *
 * Basic summary:
 *
 * - Reads data from ADS1115 ADCs (addresses 0x48, 0x49, 0x4A)
 * - Sends data to Thingspeak service over GPRS
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

#include "DLUtility.h"
#include "DLSettings.h"
#include "DLService.h"
#include "DLHTTP.h"
#include "DLNetwork.h"
#include "DLDataField.h"
#include "DLService.ThingSpeak.h"
#include "DLSensor.ADS1x1x.h"
#include "TaskAction.h"

#define FIELD_COUNT (12)

// Pointers to fuctionality objects
static ServiceInterface * s_thingSpeakService;
static NetworkInterface * s_gprsConnection;

static ADS1115 s_ADCs[] = {
    ADS1115(0x48),
    ADS1115(0x49),
    ADS1115(0x4A)
};

// In-RAM data storage
static NumericDataField s_dataFields[FIELD_COUNT] = {
    NumericDataField(VOLTAGE, 1),
    NumericDataField(VOLTAGE, 1),
    NumericDataField(VOLTAGE, 1),
    NumericDataField(VOLTAGE, 1),
    NumericDataField(CURRENT, 1),
    NumericDataField(CURRENT, 1),
    NumericDataField(CURRENT, 1),
    NumericDataField(CURRENT, 1),
    NumericDataField(CURRENT, 1),
    NumericDataField(CURRENT, 1),
    NumericDataField(CURRENT, 1),
    NumericDataField(CURRENT, 1)
};

static float s_uploadData[FIELD_COUNT];

static void updateUploadData(void)
{
    uint8_t i;
    for(i = 0; i < FIELD_COUNT; ++i)
    {
        s_uploadData[i] = s_dataFields[i].getData(0);
    }
}

// Tasks
TaskAction remoteUploadTask(remoteUploadTaskFn, 1000 * 10, INFINITE_TICKS);
void remoteUploadTaskFn(void)
{

    readFromADCs();
    
    if (!s_gprsConnection->isConnected())
    {
        Serial.println("GPRS not connected. Attempting new connection.");
        tryConnection();
    }

    // Try to upload to ThingSpeak
    Serial.print("Attempting upload to ");
    Serial.println(s_thingSpeakService->getURL());

    updateUploadData();

    char requestBuffer[1024];
    char responseBuffer[200] = "";
    s_thingSpeakService->createPostAPICall(requestBuffer, s_uploadData, FIELD_COUNT, 1024, NULL);

    Serial.print("Request '");
    Serial.print(requestBuffer);
    Serial.println("'");

    if (s_gprsConnection->sendHTTPRequest(s_thingSpeakService->getURL(), requestBuffer, responseBuffer))
    {
        Serial.print("Got response (");   
        Serial.print(strlen(responseBuffer));
        Serial.print(" bytes):");   
        Serial.println(responseBuffer);
    }
    else
    {
        Serial.print("Could not connect to ");
        Serial.println(s_thingSpeakService->getURL());
    }
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

    Settings_setString(GPRS_APN, "giffgaff.com");
    Settings_setString(GPRS_USERNAME, "giffgaff");
    Settings_setString(GPRS_PASSWORD, "");

    // Thingspeak URL choices (for development)
    Settings_setString(THINGSPEAK_URL, "api.thingspeak.com"); // Thingspeak hosted services

    // Thingspeak API keys
    Settings_setString(THINGSPEAK_API_KEY, "0CKUYB7VJI2I9WG9"); // Mouse's API key
    
    s_thingSpeakService = Service_GetService(SERVICE_THINGSPEAK);
    s_gprsConnection = Network_GetNetwork(NETWORK_INTERFACE_LINKITONE_GPRS);
}

void loop()
{
    remoteUploadTask.tick();
}
