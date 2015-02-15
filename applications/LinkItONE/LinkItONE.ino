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
 * LinkIt One Includes
 */
#include <LGPRS.h>
#include <LSD.h>
#include <LStorage.h>

/*
 * DataLogger Includes
 */

#include "DLSettings.h"
#include "DLService.h"
#include "DLNetwork.h"
#include "DLDataField.h"
#include "DLService.ThingSpeak.h"
#include "DLLocalStorage.h"

static ServiceInterface * s_thingSpeakService;
static NetworkInterface * s_gprsConnection;
static LocalStorageInterface * s_sdCard;

static DataField s_dataFields[] = {
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
            s_dataFields[f].StoreData(randomData);
        }
        else
        {
            // Other fields are random current between 0 and 10A
            randomData = (float)random(0, 1000)/100.0;
        }

        s_dataFields[f].StoreData(randomData);
        s_thingSpeakService->SetField(f, &s_dataFields[f]);
    }
}

void uploadData(void)
{
    // Try to upload to ThingSpeak
    Serial.print("Connect to ");
    Serial.println(Thingspeak::THINGSPEAK_URL);

    char request_buffer[200];
    char response_buffer[200];
    s_thingSpeakService->CreateGetAPICall(request_buffer);

    Serial.print("Request '");
    Serial.print(request_buffer);
    Serial.println("'");

    if (s_gprsConnection->HTTPGet(Thingspeak::THINGSPEAK_URL, request_buffer, response_buffer))
    {
        Serial.println("connected");
    }
    else
    {
        Serial.println("connection failed");
    }

}

void setup()
{
    // setup Serial port
    Serial.begin(115200);

    delay(10000);

    Settings_SetString(GPRS_APN, "giffgaff.com");
    Settings_SetString(GPRS_USERNAME, "giffgaff");
    Settings_SetString(GPRS_PASSWORD, "");
    Settings_SetString(THINGSPEAK_API_KEY, "0CKUYB7VJI2I9WG9");

    s_thingSpeakService = Service_GetService(SERVICE_THINGSPEAK);
    s_gprsConnection = Network_GetNetwork(NETWORK_INTERFACE_LINKITONE_GPRS);
    s_sdCard = LocalStorage_GetLocalStorageInterface(LINKITONE_SD_CARD);

    Serial.println("Attach to GPRS network by auto-detect APN setting");
    s_gprsConnection->tryConnection(5);
}

void loop()
{

    createFakeData();
    uploadData();

    delay(30000);
}
