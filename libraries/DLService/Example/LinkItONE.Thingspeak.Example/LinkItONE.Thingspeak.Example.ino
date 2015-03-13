/*
 * LinkIT ONE
 *
 * CSV Bulk upload example on the LinkItONE platform.
 *
 * See README.md for full details
 *
 * Basic summary:
 *
 * - Sends CSV data to thingspeak service
 * - Uses Linkit One platform. Data connection required.
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

#include "DLDataField.h"
#include "DLUtility.h"
#include "DLSettings.h"
#include "DLService.h"
#include "DLHTTP.h"
#include "DLNetwork.h"
#include "DLService.ThingSpeak.h"

// Pointers to fuctionality objects
static ServiceInterface * s_thingSpeakService;
static NetworkInterface * s_gprsConnection;

// Example CSV data
static char csvData[] = 
"2015-02-13 07:12:22 +0000,1,43.478,51.752,4.90,5.23,9.23,2.84\r\n\
2015-02-13 07:12:52 +0000,2,49.321,54.782,9.63,5.01,7.30,8.63\r\n\
2015-02-13 07:13:22 +0000,3,51.023,42.647,7.57,6.89,8.24,0.52\r\n\
2015-02-13 07:13:52 +0000,4,54.194,59.884,7.68,9.67,5.35,6.02\r\n\
";

void doBulkUpload(void)
{
    if (!s_gprsConnection->isConnected())
    {
        Serial.println("GPRS not connected. Attempting new connection.");
        tryConnection();
    }

    // Try to upload to ThingSpeak
    Serial.print("Attempting bulk upload to ");
    Serial.println(s_thingSpeakService->getURL());

    char request_buffer[1024];
    char response_buffer[200] = "";
    s_thingSpeakService->createBulkUploadCall(request_buffer, 1024, csvData, "linkitone.example.csv");

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

    Settings_setString(GPRS_APN, "everywhere");
    Settings_setString(GPRS_USERNAME, "eesecure");
    Settings_setString(GPRS_PASSWORD, "secure");

    // Thingspeak URL choices (for development)
    Settings_setString(THINGSPEAK_URL, "agile-headland-8076.herokuapp.com"); // Mouse's heroku instance

    // Thingspeak API keys
    Settings_setString(THINGSPEAK_API_KEY, "IZ2O45C3BM257VCH"); // Mouse's API key
    
    s_thingSpeakService = Service_GetService(SERVICE_THINGSPEAK);
    s_gprsConnection = Network_GetNetwork(NETWORK_INTERFACE_LINKITONE_GPRS);

    doBulkUpload();
}

void loop()
{
    
}
