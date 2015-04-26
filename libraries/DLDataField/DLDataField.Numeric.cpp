/*
 * DLDataField.String.cpp
 * 
 * Stores single item of string data and provides get/set functionality
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
#include "DLDataField.h"
#include "DLUtility.h"

NumericDataField::NumericDataField(FIELD_TYPE type, void * fieldData) : DataField(type)
{
    m_conversionData = fieldData;
}

NumericDataField::~NumericDataField()
{
    delete[] m_data;
}

void NumericDataField::setDataSizes(uint32_t N, uint32_t averagerN)
{
    if (N == 0 || averagerN == 0) { return; }

    setSize(N);
    
    m_data = new float[N];
    
    if (m_data)
    {
        fillArray(m_data, 0.0f, N);
    }

    m_averager = new Averager<int32_t>(averagerN);
}

float NumericDataField::getRawData(uint32_t index)
{
    index = getRealReadIndex(index);
    return m_data[index];
}

float NumericDataField::getConvData(uint32_t index)
{
    float data = getRawData(index);

    if (m_conversionData)
    {
        switch (m_fieldType)
        {
        case VOLTAGE:
            data = CONV_VoltsFromRaw(data, (VOLTAGECHANNEL*)m_conversionData);
            break;
        case CURRENT:
            data = CONV_AmpsFromRaw(data, (CURRENTCHANNEL*)m_conversionData);
            break;
        default:
            break;
        }
    }

    return data;
}

void NumericDataField::storeData(int32_t data)
{
    m_averager->newData(data);
    if (m_averager->full())
    {
        m_data[getWriteIndex()] = (float)m_averager->getFloatAverage();
        incrementIndexes();
        m_averager->reset(NULL);
    }
}

void NumericDataField::getDataAsString(char * buf, char const * const fmt, uint8_t index)
{
    index = getRealReadIndex(index);
    sprintf(buf, fmt, m_data[index]); // Write data point to buffer
}

#ifdef TEST
void NumericDataField::printContents(void)
{
    uint8_t i;
    for (i = 0; i <= m_maxIndex; ++i)
    {
        std::cout << m_data[i] << ",";
    }
    std::cout << std::endl;
}
#endif