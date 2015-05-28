/*
 * DLDataField.Manager.cpp
 * 
 * Provides management of datafields for applications
 *
 * Author: James Fowkes
 *
 * www.re-innovation.co.uk
 */

/*
 * Arduino/C++ Library Includes
 */

#ifdef ARDUINO
#include <Arduino.h>
#else 
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#endif

#ifdef TEST
#include <iostream>
#endif

/*
 * Local Application Includes
 */

#include "DLUtility.Averager.h"
#include "DLDataField.Types.h"
#include "DLDataField.h"
#include "DLDataField.Manager.h"
#include "DLSettings.DataChannels.h"
#include "DLUtility.h"
#include "DLUtility.ArrayFunctions.h"

DataFieldManager::DataFieldManager(uint32_t dataSize, uint32_t averagerSize)
{
    m_dataSize = dataSize;
    m_averagerSize = averagerSize;
    m_count = 0;

    uint8_t i = 0;
    for (i = 0; i < MAX_FIELDS; i++)
    {
        m_fields[i] = NULL;
    }
}

uint8_t DataFieldManager::fieldCount()
{
    return m_count;
}

bool DataFieldManager::addField(NumericDataField * field)
{
    if (!field) { return false; }

    if (m_count == MAX_FIELDS) { return false; }

    field->setDataSizes(m_dataSize, m_averagerSize);

    m_fields[m_count] = field;
    m_channelNumbers[m_count] = field->getChannelNumber();
    m_count++;

    return true;
}

bool DataFieldManager::addField(StringDataField * field)
{
    if (!field) { return false; }

    if (m_count == MAX_FIELDS) { return false; }

    m_fields[m_count] = field;
    m_channelNumbers[m_count] = field->getChannelNumber();

    m_count++;
    return true;
}

DataField * DataFieldManager::getChannel(uint8_t channel)
{
    int32_t actualIndex = indexOf(m_channelNumbers, (uint32_t)channel, m_count);
    return actualIndex >= 0 ? m_fields[actualIndex] : NULL;
}


DataField * DataFieldManager::getField(uint8_t index)
{
    return m_fields[index];
}

DataField ** DataFieldManager::getFields(void)
{
    return m_fields;
}

uint32_t DataFieldManager::writeHeadersToBuffer(char * buffer, uint8_t bufferLength)
{
    if (!buffer) { return 0; }

    uint8_t i;
    
    FixedLengthAccumulator headerAccumulator(buffer, bufferLength);

    for (i = 0; i < m_count; ++i)
    {
        headerAccumulator.writeString(m_fields[i]->getTypeString());
        if (!lastinloop(i, m_count))
        {
            headerAccumulator.writeString(", ");
        }
    }

    headerAccumulator.writeString("\r\n");

    return headerAccumulator.length();
}

void DataFieldManager::setupAllValidChannels(void)
{
    uint8_t ch;
    NumericDataField * field;
    FIELD_TYPE type;
    void * data;

    uint32_t maxChannels = Settings_GetMaxChannels();
    for (ch = 1; ch < maxChannels; ch++)
    {
        if (Settings_ChannelSettingIsValid(ch))
        {
            type = Settings_GetChannelType(ch);
            data = Settings_GetData(ch);

            switch(type)
            {
            case VOLTAGE:
            case CURRENT:
            case TEMPERATURE_C:
            case TEMPERATURE_K:
            case TEMPERATURE_F:
                field = new NumericDataField(type, data, ch);
                #ifdef TEST
                std::cout << "Adding channel " << (int)ch << ", type " << field->getTypeString() << std::endl;
                #endif
                addField(field);
                break;
            default:
                break;
            }
        }
    }
}

bool DataFieldManager::hasData(void)
{
    uint8_t i = 0;
    bool atLeastOneFieldHasData = false;
    for (i = 0; i < m_count; i++)
    {
        atLeastOneFieldHasData |= m_fields[i]->hasData();
    }
    return atLeastOneFieldHasData;
}

uint32_t * DataFieldManager::getChannelNumbers(void)
{
    return m_channelNumbers;
}