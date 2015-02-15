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

#include "DLDataField.h"
#include "DLService.h"
#include "DLService.thingspeak.h"

/*
 * Public static class members
 */

const char Thingspeak::THINGSPEAK_URL[] = "api.thingspeak.com";
const char Thingspeak::THINGSPEAK_GET_PATH[] = "/update";

/*
 * Public Class Functions
 */

Thingspeak::Thingspeak(char const * const key)
{
    if (key)
    {
        memcpy(m_key, key, strlen(key));
    }
    
    uint8_t i;
    for (i = 0; i < 6; i++)
    {
        m_data[i] = 0.0;
    }
}

Thingspeak::~Thingspeak() {}

void Thingspeak::SetField(uint8_t fieldIndex, DataField * pDataField)
{
    if (fieldIndex > 5) { return; }
    
    m_data[fieldIndex] = pDataField->GetDataAsFloat();
}

uint16_t Thingspeak::CreateGetAPICall(char * buffer)
{
    return CreateGetAPICall(buffer, NULL);
}

uint16_t Thingspeak::CreateGetAPICall(char * buffer, char const * const time)
{
    if (!buffer) { return 0; }
    if (!m_key) { return 0; }
    
    uint16_t index = 0;
    
    // Copy the thingspeak update path into buffer
    index += sprintf(&buffer[index], THINGSPEAK_GET_PATH);
    index += sprintf(&buffer[index], "?");
    index += CreateGetAPIParamsString(&buffer[index], time);
    
    return index;
}

uint16_t Thingspeak::CreateGetAPIParamsString(char * buffer)
{
    return CreateGetAPIParamsString(buffer, NULL);
}

uint16_t Thingspeak::CreateGetAPIParamsString(char * buffer, char const * const time)
{
    if (!buffer) { return 0; }
    if (!m_key) { return 0; }
    
    uint16_t index = 0;
    index += sprintf(&buffer[index], "key=%s", m_key);
    
    uint8_t field = 1;
     // Use 1 to _MAX_FIELDS+1 indexing as thingspeak labels its fields field1-field6
    for (field = 1; field < (_MAX_FIELDS+1); field++)
    {
        index += sprintf(&buffer[index], "&field%d=%.5f", field, m_data[field-1]);
    }
 
    // Copy the time into the buffer (if provided)
    if (time)
    {
        index += sprintf(&buffer[index], "&created_at=%s", time);
    }
    
    return index;  
}
