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

#include <stdint.h>

#include "datalogger.h"

static ServiceInterface * s_thingSpeakService;
static NetworkInterface * s_gprsService;
static LGPRSClient client;

static DataField dataFields[] = {
    DataField(VOLTAGE),
    DataField(VOLTAGE),
    DataField(CURRENT),
    DataField(CURRENT),
    DataField(CURRENT),
    DataField(CURRENT)
};

void createFakeData(void)
{
    float randomData = 0.0;
    uint8_t f = 0;

    for (f = 0; f < 6; f++)
    {
        if (f < 2)
        {
            // First two fields are random voltage between 40V and 60V
            randomData = (float)random(4000, 6000)/100.0;
            dataFields[f].StoreData(&randomData);
        }
        else
        {
            // Other fields are random current between 0 and 10A
            randomData = (float)random(0, 1000)/100.0;
        }

        dataFields[f].StoreData(&randomData);
        s_thingSpeakService->Set(f, &dataFields[f]);
    }
}

void uploadData(void)
{
    // Try to upload to ThingSpeak
    Serial.print("Connect to ");
    Serial.println(Thingspeak::THINGSPEAK_URL);

    char request_buffer[200];
    s_thingSpeakService->CreateGetAPICall(request_buffer);

    Serial.print("Request '");
    Serial.print(request_buffer);
    Serial.println("'");

    if (client.connect(Thingspeak::THINGSPEAK_URL, HTTP_PORT))
    {
        Serial.println("connected");
        // Make a HTTP request:
        client.print("GET ");
        client.print(request_buffer);
        client.println(" HTTP/1.1");
        client.print("Host: ");
        client.println(Thingspeak::THINGSPEAK_URL);
        client.println("Connection: close");
        client.println();
    }
    else
    {
        // if you didn't get a connection to the server:
        Serial.println("connection failed");
    }

}

void readResponse(void)
{
    // if there are incoming bytes available
    // from the server, read them and print them:
    if (client.available())
    {
        char c = client.read();
        Serial.print(c);
    }

    // if the server's disconnected, stop the client:
    if (!client.available() && !client.connected())
    {
        Serial.println();
        Serial.println("disconnecting.");
        client.stop();
    }
}

void setup()
{
    // setup Serial port
    Serial.begin(115200);

    delay(10000);

    SetString(GRPS_APN, "giffgaff.com");
    SetString(GPRS_USERNAME, "giffgaff");
    SetString(GPRS_PASSWORD, "");
    SetString(THINGSPEAK_API_KEY, "0CKUYB7VJI2I9WG9");

    s_thingSpeakService = GetService(SERVICE_THINGSPEAK);
    s_gprsService = GetInterface(NETWORK_INTERFACE_LINKITONE_GPRS);
    Serial.println("Attach to GPRS network by auto-detect APN setting");
    s_gprsService->tryConnection(5);
}

void loop()
{

    createFakeData();
    uploadData();
    readResponse();

    delay(30000);
}
