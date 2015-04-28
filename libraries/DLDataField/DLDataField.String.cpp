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
#include "DLDataField.Types.h"
#include "DLDataField.h"
#include "DLUtility.h"

StringDataField::StringDataField(FIELD_TYPE type, uint8_t len, uint32_t N) : DataField(type)
{
    
    setSize(N);
    
    m_data = new char*[N];
    
    if (m_data)
    {
        uint8_t i = 0;
        for (i = 0; i < N; i++)
        {
            m_data[i] = new char[len];
        }
    }

    m_maxLength = len;
}

StringDataField::~StringDataField()
{
    uint8_t i = 0;
    for (i = 0; i < m_maxIndex; i++)
    {
        delete[] m_data[i];
    }

    delete[] m_data;
}

void StringDataField::storeData(char const * data)
{
    prePush();
    strncpy_safe(m_data[getWriteIndex()], data, m_maxLength);
    postPush();
}

char * StringDataField::getData(bool alsoRemove)
{
    char * data = m_data[ getTailIndex() ];
    if (alsoRemove) { pop(); }
    return data;
}

void StringDataField::copy(char * buf, bool alsoRemove)
{
    char * data = m_data[ getTailIndex() ];
    strncpy_safe(buf, data, m_maxLength);
    if (alsoRemove) { pop(); }
}

