/*
 * DLService.thingspeak.cpp
 *
 * James Fowkes
 * 06 February 2015
 *
 * www.re-innovation.co.uk
 */
 
#ifdef ARDUINO
#include <Arduino.h>
#else
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#endif

#include "DLUtility.h"
#include "DLDataField.h"
#include "DLService.h"
#include "DLService.thingspeak.h"
#include "DLHTTP.h"

/*
 * Public static class members
 */

const char THINGSPEAK_DEFAULT_URL[] = "api.thingspeak.com"; 

const char Thingspeak::THINGSPEAK_UPDATE_PATH[] = "/update";
const char Thingspeak::THINGSPEAK_BULK_UPDATE_PATH[] = "/update_csv";

// The boundary can be any string the is guranteed not to appear in the data
// This is typically implemented as a string of hyphens followed by a random HEX string.
// For ease of implementation here, the "random" hex string is fixed.
// If a random HEX string is required, this is a trivial future addition.

// It is #define here to that it can be used either as a const char * or as an in-place define

#define __THINGSPEAK_MULTIPART_BOUNDARY_STR__ "----------------------9f1bb96494379c3e"

const char Thingspeak::THINGSPEAK_MULTIPART_BOUNDARY[] = __THINGSPEAK_MULTIPART_BOUNDARY_STR__;

RequestBuilder builder;

/*
 * Public Class Functions
 */

Thingspeak::Thingspeak(char const * const url, char const * const key)
{
    if (url)
    {
        memcpy(m_url, url, strlen(url));   
    }
    else
    {
        memcpy(m_url, THINGSPEAK_DEFAULT_URL, strlen(THINGSPEAK_DEFAULT_URL));   
    }

    memcpy(m_key, key, strlen(key)); 
}

Thingspeak::~Thingspeak() {}

char * Thingspeak::getURL(void)
{
    return m_url;
}

uint16_t Thingspeak::createPostAPICall(
    char * buffer, float * data, uint8_t nFields, uint16_t maxSize)
{
    return createPostAPICall(buffer, data, nFields, maxSize, NULL);
}

uint16_t Thingspeak::createPostAPICall(
    char * buffer, float * data, uint8_t nFields, uint16_t maxSize, char const * const pTime)
{
    if (!buffer) { return 0; }
    if (!m_key) { return 0; }
    
    char m_body[maxSize];

    builder.reset();
    builder.setMethodAndURL("POST", THINGSPEAK_UPDATE_PATH);

    builder.putHeader("Host", m_url);
    builder.putHeader("Connection", "Close");
    builder.putHeader("X-THINGSPEAKAPIKEY", m_key);
    builder.putHeader("Content-Type", "application/x-www-form-urlencoded");

    uint8_t field = 0;
    uint8_t index = 0;
    for (field = 0; field < nFields; field++)
    {
        // Make the data string
        index += sprintf(&m_body[index], "%d=%.5f", field+1, data[field]);
        if (!lastinloop(field, nFields))
        {
            m_body[index++] = '&';
        }
    }

    // Copy the time into the buffer (if provided)
    if (pTime)
    {
        index += sprintf(&m_body[index], "created_at=%s", pTime);
    }

    builder.putBody(m_body);

    builder.writeToBuffer(buffer, maxSize, true);

    return index;
}

/* Creates a bulk upload call for thingspeak.
 * Args:
    buffer - the buffer into which the request is written
    maxSize - the maximum size of the buffer
    csvData - a pointer to the CSV data. Expected CSV line format is:
        creation date/time, entry id, field1 data, field2 data... fieldN data\r\n
    filename - The name of the file fro, which the CSV data has been pulled
*/

void Thingspeak::createBulkUploadCall(char * buffer, uint16_t maxSize, const char * csvData, const char * filename, uint8_t nFields)
{
    if (!buffer) { return; }
    if (!m_key) { return; }

    /* Creates the HTTP headers for the bulk upload */
    char m_body[maxSize];
    FixedLengthAccumulator bodyAccumulator(m_body, maxSize);
    
    builder.reset();
    builder.setMethodAndURL("POST", THINGSPEAK_BULK_UPDATE_PATH);

    builder.putHeader("Host", m_url);

    builder.putHeader("Content-Type", "multipart/form-data; boundary=" __THINGSPEAK_MULTIPART_BOUNDARY_STR__);

    // Write the API key
    bodyAccumulator.writeString("--");
    bodyAccumulator.writeLine(THINGSPEAK_MULTIPART_BOUNDARY);
    bodyAccumulator.writeLine("Content-Disposition: form-data; name=\"api_key\"");
    bodyAccumulator.writeLine("");
    bodyAccumulator.writeLine(m_key);
    bodyAccumulator.writeString("--");
    bodyAccumulator.writeLine(THINGSPEAK_MULTIPART_BOUNDARY);

    // Write the CSV data
    bodyAccumulator.writeString("Content-Disposition: form-data; name=\"upload[csv]\"; filename=\"");
    bodyAccumulator.writeString(filename);
    bodyAccumulator.writeLine("\"");
    bodyAccumulator.writeLine("Content-Type: application/octet-stream");

    bodyAccumulator.writeLine("");

    putCSVUploadHeaders(&bodyAccumulator, nFields);
    bodyAccumulator.writeString(csvData);
    bodyAccumulator.writeLine("");
    bodyAccumulator.writeString("--");
    bodyAccumulator.writeString(THINGSPEAK_MULTIPART_BOUNDARY);
    bodyAccumulator.writeString("--");

    builder.putBody(m_body);

    builder.writeToBuffer(buffer, maxSize, true);
}

void Thingspeak::putCSVUploadHeaders(FixedLengthAccumulator * accumulator, uint8_t nFields)
{
    if (!accumulator) { return; }

    accumulator->writeString("created_at,entry_id,");

    uint8_t field = 0;

    char fieldBuffer[5];

    for (field = 1; field < nFields + 1; field++)
    {
        accumulator->writeString("field");
    
        sprintf(fieldBuffer, "%d", field);

        accumulator->writeString(fieldBuffer);

        if (!lastinloop(field, nFields + 1))
        {
            accumulator->writeChar(',');
        }
    }
    accumulator->writeString("\r\n");
}
