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

#include "app.data_conversion.h"
#include "app.upload_manager.h"

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
#define AVERAGER_COUNT (12)

// ...plus three more fields for temperature data
#define THERMISTOR_COUNT (3)

#define FIELD_COUNT (AVERAGER_COUNT + THERMISTOR_COUNT)

#define ADC_READS_PER_SECOND (10)
#define AVERAGING_PERIOD_SECONDS (1)

#define MS_PER_ADC_READ (1000 / ADC_READS_PER_SECOND)

#define BULK_UPLOAD_BUFFER_SIZE (16384)

static Averager<uint16_t> s_averagers[] = {
    Averager<uint16_t>(ADC_READS_PER_SECOND * AVERAGING_PERIOD_SECONDS),
    Averager<uint16_t>(ADC_READS_PER_SECOND * AVERAGING_PERIOD_SECONDS),
    Averager<uint16_t>(ADC_READS_PER_SECOND * AVERAGING_PERIOD_SECONDS),
    Averager<uint16_t>(ADC_READS_PER_SECOND * AVERAGING_PERIOD_SECONDS),
    Averager<uint16_t>(ADC_READS_PER_SECOND * AVERAGING_PERIOD_SECONDS),
    Averager<uint16_t>(ADC_READS_PER_SECOND * AVERAGING_PERIOD_SECONDS),
    Averager<uint16_t>(ADC_READS_PER_SECOND * AVERAGING_PERIOD_SECONDS),
    Averager<uint16_t>(ADC_READS_PER_SECOND * AVERAGING_PERIOD_SECONDS),
    Averager<uint16_t>(ADC_READS_PER_SECOND * AVERAGING_PERIOD_SECONDS),
    Averager<uint16_t>(ADC_READS_PER_SECOND * AVERAGING_PERIOD_SECONDS),
    Averager<uint16_t>(ADC_READS_PER_SECOND * AVERAGING_PERIOD_SECONDS),
    Averager<uint16_t>(ADC_READS_PER_SECOND * AVERAGING_PERIOD_SECONDS)
};

// Medium term in-RAM data storage and remote storage
#define STORE_TO_SD_EVERY_N_AVERAGES (10)
#define STORE_TO_SD_INTERVAL_MS (STORE_TO_SD_EVERY_N_AVERAGES * AVERAGING_PERIOD_SECONDS * 1000)
#define UPLOAD_EVERY_N_AVERAGES (10)
#define UPLOAD_INTERVAL_MS (UPLOAD_EVERY_N_AVERAGES * AVERAGING_PERIOD_SECONDS * 1000)

static NumericDataField<float> s_dataFields[] = {
    NumericDataField<float>(VOLTAGE, STORE_TO_SD_EVERY_N_AVERAGES),
    NumericDataField<float>(VOLTAGE, STORE_TO_SD_EVERY_N_AVERAGES),
    NumericDataField<float>(VOLTAGE, STORE_TO_SD_EVERY_N_AVERAGES),
    NumericDataField<float>(VOLTAGE, STORE_TO_SD_EVERY_N_AVERAGES),
    NumericDataField<float>(CURRENT, STORE_TO_SD_EVERY_N_AVERAGES),
    NumericDataField<float>(CURRENT, STORE_TO_SD_EVERY_N_AVERAGES),
    NumericDataField<float>(CURRENT, STORE_TO_SD_EVERY_N_AVERAGES),
    NumericDataField<float>(CURRENT, STORE_TO_SD_EVERY_N_AVERAGES),
    NumericDataField<float>(CURRENT, STORE_TO_SD_EVERY_N_AVERAGES),
    NumericDataField<float>(CURRENT, STORE_TO_SD_EVERY_N_AVERAGES),
    NumericDataField<float>(CURRENT, STORE_TO_SD_EVERY_N_AVERAGES),
    NumericDataField<float>(CURRENT, STORE_TO_SD_EVERY_N_AVERAGES),
    NumericDataField<float>(TEMPERATURE_C, STORE_TO_SD_EVERY_N_AVERAGES),
    NumericDataField<float>(TEMPERATURE_C, STORE_TO_SD_EVERY_N_AVERAGES),
    NumericDataField<float>(TEMPERATURE_C, STORE_TO_SD_EVERY_N_AVERAGES)
};

static CONVERSION_FN s_conversionFunctions[] = 
{
    channel01Conversion,
    channel02Conversion,
    channel03Conversion,
    channel04Conversion,
    channel05Conversion,
    channel06Conversion,
    channel07Conversion,
    channel08Conversion,
    channel09Conversion,
    channel10Conversion,
    channel11Conversion,
    channel12Conversion,
};

static LocalStorageInterface * s_sdCard;
static FILE_HANDLE s_fileHandle;
static uint32_t s_entryID = 0;

static TM s_time;
static TM s_lastSDTimestamp;

static uint16_t s_fileCountToday = 0;

static char csvData[BULK_UPLOAD_BUFFER_SIZE];
static char request_buffer[BULK_UPLOAD_BUFFER_SIZE];
    
static void writeTimestampToOpenFile(void)
{
    char buffer[30];
    time_increment_seconds(&s_lastSDTimestamp);
    CSV_writeTimestampToBuffer(&s_lastSDTimestamp, buffer);
    s_sdCard->write(s_fileHandle, buffer);
    s_sdCard->write(s_fileHandle, ", ");
}

static void writeEntryIDToOpenFile(void)
{
    char buffer[30];
    s_entryID++;
    sprintf(buffer, "%d, ", s_entryID);
    s_sdCard->write(s_fileHandle, buffer);
}

static void readAllDataFromCurrentFile(char * buffer)
{
    char const * const pFilename = Filename_get();
    uint32_t c = 0;

    if (s_sdCard->fileExists(pFilename))
    {
        s_fileHandle = s_sdCard->openFile(pFilename, false);

        // Discard the first line
        s_sdCard->readLine(s_fileHandle,  buffer, BULK_UPLOAD_BUFFER_SIZE, false);

        while (!s_sdCard->endOfFile(s_fileHandle))
        {
            c += s_sdCard->readLine(s_fileHandle, &buffer[c], BULK_UPLOAD_BUFFER_SIZE-c, false);
        }

        s_sdCard->closeFile(s_fileHandle);
        
        Serial.println("Read from file:");
        Serial.print(buffer);
        Serial.println("");
    }
    else
    {
        Serial.print("Could not open ");
        Serial.print(pFilename);
        Serial.println(" for reading.");
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

static void setNewFilename(void)
{
    Time_GetTime(&s_time, TIME_PLATFORM);

    s_fileCountToday = 0;
    Filename_setFromDate(s_time.tm_mday, s_time.tm_mon, s_time.tm_year, s_fileCountToday);

    while (s_sdCard->fileExists(Filename_get()))
    {
        Filename_setFromDate(s_time.tm_mday, s_time.tm_mon, s_time.tm_year, ++s_fileCountToday);
    }

    Serial.print("Starting with file index ");
    Serial.println(s_fileCountToday);
}

static TaskAction averageAndStoreTask(averageAndStoreTaskFn, AVERAGING_PERIOD_SECONDS * 1000, INFINITE_TICKS);
static void averageAndStoreTaskFn(void)
{
    uint8_t field = 0;
    uint8_t i;

    for (i = 0; i < AVERAGER_COUNT; i++)
    {
        uint16_t average = s_averagers[i].getAverage();
        s_averagers[i].reset(NULL);

        float toStore;
        if (s_conversionFunctions[i])
        {
            toStore = s_conversionFunctions[i](average); 
        }
        else
        {
            toStore = (float)average;
        }
        s_dataFields[i].storeData( toStore );
    }

    // The ADC range is 0 to 5v, but thermistors are on 3.3v rail, so maximum is 1023 * 3.3/5 = 675
    for (i = 0; i < THERMISTOR_COUNT; i++)
    {
        float data = s_thermistors[i].TemperatureFromADCReading(10000.0, s_adcs[i].read(), 675);
        s_dataFields[i + AVERAGER_COUNT].storeData(data);
    }
}

static void createNewDataFile(void)
{

    setNewFilename();

    char const * const pFilename = Filename_get();

    Serial.print("Creating new file: ");
    Serial.println(pFilename);

    s_fileHandle = s_sdCard->openFile(pFilename, true);

    // Write Timestamp and Entry ID headers
    s_sdCard->write(s_fileHandle, "Timestamp, Entry ID, ");

    char csvHeaders[200];
    DataField_writeHeadersToBuffer(csvHeaders, s_dataFields, FIELD_COUNT, 200);
    s_sdCard->write(s_fileHandle, csvHeaders);
    s_entryID = 0;
    s_sdCard->closeFile(s_fileHandle);
}

static void openDataFileForToday(void)
{

    char const * const pFilename = Filename_get();

    if (s_sdCard->fileExists(pFilename))
    {
        Serial.print("Opening existing file: ");
        Serial.println(pFilename);

        s_fileHandle = s_sdCard->openFile(pFilename, true);
    }
    else
    {
        Serial.print("Expected file '");
        Serial.print(pFilename);
        Serial.print(" 'does not exist.");
    }
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

    readAllDataFromCurrentFile(csvData);

    s_thingSpeakService->createBulkUploadCall(
        request_buffer, BULK_UPLOAD_BUFFER_SIZE, csvData, Filename_get(), 15);

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

    // Start a new file after upload
    createNewDataFile();
}

static TaskAction writeToSDCardTask(writeToSDCardTaskFn, STORE_TO_SD_INTERVAL_MS, INFINITE_TICKS);
static void writeToSDCardTaskFn(void)
{
    Serial.println("Writing averages to SD card");
    uint8_t field = 0;
    uint8_t i, j;
    char buffer[10];

    openDataFileForToday();

    if (s_fileHandle != INVALID_HANDLE)
    {
        for (i = 0; i < STORE_TO_SD_EVERY_N_AVERAGES; ++i)
        {
            writeTimestampToOpenFile();
            writeEntryIDToOpenFile();
            
            for (j = 0; j < FIELD_COUNT; ++j)
            {
                // Write from datafield to buffer then from buffer to SD file
                s_dataFields[j].getDataAsString(buffer, "%.4f", i);

                s_sdCard->write(s_fileHandle, buffer);

                if (!lastinloop(j, FIELD_COUNT))
                {
                    s_sdCard->write(s_fileHandle, ", ");
                }
            }
            s_sdCard->write(s_fileHandle, "\r\n");
        }

        s_sdCard->closeFile(s_fileHandle);
    }
    else
    {
        Serial.print("Could not open '");
        Serial.print(Filename_get());
        Serial.println("' when trying to write data.");
    }
}

TaskAction readFromADCsTask(readFromADCsTaskFn, MS_PER_ADC_READ, INFINITE_TICKS);
void readFromADCsTaskFn(void)
{
    uint8_t adc = 0;
    uint8_t ch = 0;
    uint8_t field = 0;
    for (adc = 0; adc < 3; adc++)
    {
        for (ch = 0; ch < 4; ch++)
        {
            field = (adc*4)+ch;
            s_averagers[field].newData(s_ADCs[adc].readADC_SingleEnded(ch));
        }
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

    s_sdCard = LocalStorage_GetLocalStorageInterface(LINKITONE_SD_CARD);
    s_sdCard->setEcho(true);

    createNewDataFile();

    Time_GetTime(&s_lastSDTimestamp, TIME_PLATFORM);

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
    buildTime.tm_hour = 17;
    buildTime.tm_min = 47;
    buildTime.tm_sec = 0;
    
    Time_SetPlatformTime(&buildTime);
    
    readFromADCsTask.ResetTicks();
    averageAndStoreTask.ResetTicks();
    writeToSDCardTask.ResetTicks();
    remoteUploadTask.ResetTicks();
}

void loop()
{
    readFromADCsTask.tick();
    averageAndStoreTask.tick();
    writeToSDCardTask.tick();
    remoteUploadTask.tick();
}
