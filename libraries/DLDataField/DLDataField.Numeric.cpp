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

#include "DLDataField.h"
#include "DLUtility.h"

NumericDataField::NumericDataField(FIELD_TYPE type, uint32_t N) : DataField(type, N)
{
    m_data = new float[N];
    
    if (m_data)
    {
        fillArray(m_data, 0.0f, N);
    }
}

NumericDataField::~NumericDataField()
{
    delete[] m_data;
}

float NumericDataField::getData(uint32_t index)
{
    index = getRealReadIndex(index);
    return m_data[index];
}

template <typename T>
void NumericDataField::storeData(T data)
{
    m_data[getWriteIndex()] = (float)data;
    incrementIndexes();
}

void NumericDataField::getDataAsString(char * buf, char const * const fmt, uint8_t index)
{
    sprintf(buf, fmt, m_data[index]); // Write data point to buffer
}

template void NumericDataField::storeData(uint8_t);
template void NumericDataField::storeData(uint16_t);
template void NumericDataField::storeData(uint32_t);
template void NumericDataField::storeData(int8_t);
template void NumericDataField::storeData(int16_t);
template void NumericDataField::storeData(int32_t);
template void NumericDataField::storeData(float);

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