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

#include "DLDataField.h"
#include "DLDataField.Manager.h"
#include "DLUtility.h"

DataFieldManager::DataFieldManager()
{
    m_count = 0;

    uint8_t i = 0;
    for (i = 0; i < MAX_FIELDS; i++)
    {
        m_fields[i] = NULL;
    }
}

uint8_t DataFieldManager::count()
{
    return m_count;
}

bool DataFieldManager::addField(DataField * field)
{
    if (!field) { return false; }

    if (m_count == MAX_FIELDS) { return false; }

    m_fields[m_count++] = field;
    return true;
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