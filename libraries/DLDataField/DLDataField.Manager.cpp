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
 * Datalogger Library Includes
 */

#include "DLUtility.Averager.h"
#include "DLDataField.Types.h"
#include "DLDataField.h"
#include "DLDataField.Manager.h"
#include "DLSettings.Reader.Errors.h"
#include "DLSettings.DataChannels.h"
#include "DLUtility.h"
#include "DLUtility.ArrayFunctions.h"
#include "DLPlatform.h"

DataFieldManager::DataFieldManager(uint32_t dataSize, uint32_t averagerSize)
{
    m_dataSize = dataSize;
    m_averagerSize = averagerSize;
    m_fieldCount = 0;
    m_dataCount = 0;

    uint8_t i = 0;
    for (i = 0; i < MAX_FIELDS; i++)
    {
        m_fields[i] = NULL;
    }
}

uint8_t DataFieldManager::fieldCount()
{
    return m_fieldCount;
}

/*
 * addField
 *
 * Add a field to the manager.
 * Stores field pointer in next free location in m_fields.
 * Channel number is stored in m_channelNumbers.
 */
bool DataFieldManager::addField(NumericDataField * field)
{
    if (!field) { return false; }

    if (m_fieldCount == MAX_FIELDS) { return false; }

    // The field might need extra setup based on the datatype/sensor and platform.
    // The platform interface takes care of that.
    PLATFORM_specialFieldSetup(field);

    field->setDataSizes(m_dataSize, m_averagerSize);

    m_fields[m_fieldCount] = field;
    m_channelNumbers[m_fieldCount] = field->getChannelNumber();
    m_fieldCount++;

    return true;
}

bool DataFieldManager::addField(StringDataField * field)
{
    if (!field) { return false; }

    if (m_fieldCount == MAX_FIELDS) { return false; }

    m_fields[m_fieldCount] = field;
    m_channelNumbers[m_fieldCount] = field->getChannelNumber();

    m_fieldCount++;
    return true;
}

void DataFieldManager::storeDataArray(int32_t * data)
{
    uint16_t field = 0;
    bool newAverageStored = false;
    for (field = 0; field < m_fieldCount; field++)
    {
        NumericDataField* pField = (NumericDataField*)m_fields[field];

        if (pField)
        {
            newAverageStored |= pField->storeData( data[field] );
        }
    }

    if (newAverageStored) { m_dataCount++; }
}

void DataFieldManager::getDataArray(float * buffer, bool converted, bool alsoRemove)
{
    uint16_t field;
    for(field = 0; field < m_fieldCount; ++field)
    {
        if (converted)
        {
            buffer[field] = ((NumericDataField*)m_fields[field])->getConvData(alsoRemove);
        }
        else
        {
            buffer[field] = ((NumericDataField*)m_fields[field])->getRawData(alsoRemove);
        }
    }
    if (alsoRemove) { m_dataCount--; }
}

DataField * DataFieldManager::getChannel(uint8_t channel)
{
    int32_t actualIndex = indexOf(m_channelNumbers, (uint32_t)channel, m_fieldCount);
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

    for (i = 0; i < m_fieldCount; ++i)
    {
        headerAccumulator.writeString(m_fields[i]->getTypeString());
        if (!lastinloop(i, m_fieldCount))
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
    for (i = 0; i < m_fieldCount; i++)
    {
        atLeastOneFieldHasData |= m_fields[i]->hasData();
    }
    return atLeastOneFieldHasData;
}

uint32_t DataFieldManager::count(void)
{
    return m_dataCount;
}

uint32_t * DataFieldManager::getChannelNumbers(void)
{
    return m_channelNumbers;
}