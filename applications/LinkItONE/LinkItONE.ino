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
#include <LStorage.h>

/*
 * DataLogger Includes
 */

#include "DLSettings.h"
#include "DLGPS.h"
#include "DLService.h"
#include "DLNetwork.h"
#include "DLDataField.h"
#include "DLService.ThingSpeak.h"
#include "DLLocalStorage.h"
#include "DLUtility.h"
#include "DLSensor.ADS1x1x.h"

static ServiceInterface * s_thingSpeakService;
static NetworkInterface * s_gprsConnection;
static LocalStorageInterface * s_sdCard;

static ADS1115 s_ADC();

static DataField s_dataFields[] = {
    DataField(VOLTAGE),
    DataField(VOLTAGE),
    DataField(CURRENT),
    DataField(CURRENT),
    DataField(CURRENT),
    DataField(CURRENT)
};

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

static void uploadData(void)
{
    if (!s_gprsConnection->isConnected())
    {
        Serial.println("GPRS not connected. Attempting new connection.");
        tryConnection();
    }

    // Try to upload to ThingSpeak
    Serial.print("Attempting upload to ");
    Serial.println(s_thingSpeakService->getURL());

    char request_buffer[200];
    char response_buffer[200];
    s_thingSpeakService->createGetAPICall(request_buffer);

    Serial.print("Request '");
    Serial.print(request_buffer);
    Serial.println("'");

    if (s_gprsConnection->HTTPGet(s_thingSpeakService->getURL(), request_buffer, response_buffer))
    {
        Serial.print("Got response:");   
        Serial.println(response_buffer);
    }
    else
    {
        Serial.print("Could not connect to ");
        Serial.println(s_thingSpeakService->getURL());
    }

}

void setup()
{
    // setup Serial port
    Serial.begin(115200);

    delay(10000);

    Settings_setString(GPRS_APN, "giffgaff.com");
    Settings_setString(GPRS_USERNAME, "giffgaff");
    Settings_setString(GPRS_PASSWORD, "");

    //Settings_setString(THINGSPEAK_URL, "agile-headland-8076.herokuapp.com"); // Mouse's heroku instance
    //Settings_setString(THINGSPEAK_API_KEY, "IZ2O45C3BM257VCH"); // Mouse's API key

    Settings_setString(THINGSPEAK_API_KEY, "NNMOIJ91NG0MRNNX"); // My Thingspeak API key
    

    s_thingSpeakService = Service_GetService(SERVICE_THINGSPEAK);
    s_gprsConnection = Network_GetNetwork(NETWORK_INTERFACE_LINKITONE_GPRS);
    s_sdCard = LocalStorage_GetLocalStorageInterface(LINKITONE_SD_CARD);

}

void loop()
{

    createFakeData();
    uploadData();

    delay(17000);
}
