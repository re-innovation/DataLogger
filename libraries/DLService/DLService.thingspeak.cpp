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

const char Thingspeak::THINGSPEAK_UPDATE_PATH[] = "/update";

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
    
    char m_body[maxSize];

    builder.reset();
    builder.setMethodAndURL("POST", THINGSPEAK_UPDATE_PATH);

    builder.putHeader("Host", THINGSPEAK_DEFAULT_URL);
    builder.putHeader("Connection", "Close");
    builder.putHeader("X-THINGSPEAKAPIKEY", m_key);
    builder.putHeader("Content-Type", "application/x-www-form-urlencoded");

    uint8_t field = 0;
    uint8_t index = 0;
    for (field = 0; field < _MAX_FIELDS; field++)
    {
        // Make the data string
        index += sprintf(&m_body[index], "%d=%.5f", field+1, m_data[field]);
        if (!lastinloop(field, _MAX_FIELDS))
        {
            m_body[index++] = '&';
        }
    }

    // Copy the time into the buffer (if provided)
    if (time)
    {
        index += sprintf(&m_body[index], "created_at=", time);
        sprintf(&m_body[index], time);
    }

    builder.putBody(m_body);

    builder.writeToBuffer(buffer, maxSize, true);

}
