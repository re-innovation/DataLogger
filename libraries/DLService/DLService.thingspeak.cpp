/*
 * thingspeak.cpp
 *
 * James Fowkes
 * 06 February 2015
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

const char Thingspeak::THINGSPEAK_GET_PATH[] = "/update";

char s_requestBuffer[2048];
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

    uint8_t i;
    for (i = 0; i < 6; i++)
    {
        m_data[i] = 0.0;
    }
}

Thingspeak::~Thingspeak() {}

char * Thingspeak::getURL(void)
{
    return m_url;
}

void Thingspeak::setField(uint8_t fieldIndex, DataField * pDataField)
{
    if (fieldIndex > 5) { return; }
    
    m_data[fieldIndex] = pDataField->GetDataAsFloat();
}

uint16_t Thingspeak::createGetAPICall(char * buffer, uint16_t maxSize)
{
    return createGetAPICall(buffer, maxSize, NULL);
}

uint16_t Thingspeak::createGetAPICall(char * buffer, uint16_t maxSize, char const * const time)
{
    if (!buffer) { return 0; }
    if (!m_key) { return 0; }
    
    char m_fieldNames[_MAX_FIELDS][7];
    char m_dataStrs[_MAX_FIELDS][10];

    builder.reset();
    builder.setMethodAndURL("GET", THINGSPEAK_GET_PATH);

    builder.setURLParam("api_key", m_key);
    
    uint8_t field = 0;
    for (field = 0; field < _MAX_FIELDS; field++)
    {
        // Make the fieldname string
        strncpy(m_fieldNames[field], "Field", 5);
        m_fieldNames[field][5] = field + 1 + '0';
        m_fieldNames[field][6] = '\0';

        // Make the data string
        (void)sprintf(m_dataStrs[field], "%.5f", m_data[field]);

        builder.setURLParam(m_fieldNames[field], m_dataStrs[field]);
    }
 
    // Copy the time into the buffer (if provided)
    if (time)
    {
        builder.setURLParam("created_at", time);
    }

    builder.putHeader("Host", THINGSPEAK_DEFAULT_URL);
    builder.putHeader("Connection", "Close");

    builder.writeToBuffer(buffer, maxSize);

}
