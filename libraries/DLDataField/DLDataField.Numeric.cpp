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
#include "DLDataField.Conversion.h"
#include "DLDataField.h"
#include "DLUtility.h"

/*
 * Static file functions
 */

static void printVoltageData(char * buffer, VOLTAGECHANNEL * data)
{
    sprintf(buffer, "R1 = %.1f, R2 = %.1f, mVPerBit = %.4f, Offset = %.4f, Multipler = %.4f",
        data->R1, data->R2, data->mvPerBit, data->offset, data->multiplier);
}

static void printCurrentData(char * buffer, CURRENTCHANNEL * data)
{
    sprintf(buffer, "Offset = %.1f, mvPerAmp = %.1f, mVPerBit = %.4f",
        data->offset, data->mvPerAmp, data->mvPerBit);
}

static void printThermistorData(char * buffer, THERMISTORCHANNEL * data)
{
    sprintf(buffer, "Other R = %.1f, R25 = %.1f, B = %.1f, maxADC = %d, %s",
        data->otherR, data->R25, data->B, (int)data->maxADC, data->highside ? "highside" : "lowside");
}

/*
 * Public class Functions
 */
NumericDataField::NumericDataField(FIELD_TYPE type, void * fieldData, uint32_t channelNumber) : DataField(type, channelNumber)
{
    m_conversionData = fieldData;
    m_altConversionFn = NULL;
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


void NumericDataField::setAltConversion(APP_CONVERSION_FN * altConversionFn)
{
    m_altConversionFn = altConversionFn;
}

float NumericDataField::getRawData(bool alsoRemove)
{
    if (length() > 0)
    {
        float data = m_data[ getTailIndex() ];
        if (alsoRemove) { pop(); }
        return data;
    }
    else
    {
        return DATAFIELD_NO_DATA_VALUE;
    }
}

float NumericDataField::getConvData(bool alsoRemove)
{
    float data = getRawData(alsoRemove);

    if (m_conversionData)
    {
        if (m_altConversionFn)
        {
            // Conversion has been overriden for this field
            data = m_altConversionFn(data, (void*)m_conversionData);
        }
        else
        {
            switch (m_fieldType)
            {
            case VOLTAGE:
                data = CONV_VoltsFromRaw(data, (VOLTAGECHANNEL*)m_conversionData);
                break;
            case CURRENT:
                data = CONV_AmpsFromRaw(data, (CURRENTCHANNEL*)m_conversionData);
                break;
            case TEMPERATURE_C:
                data = CONV_CelsiusFromRawThermistor(data, (THERMISTORCHANNEL*)m_conversionData);
            default:
                break;
            }
        }
    }

    return data;
}

bool NumericDataField::storeData(int32_t data)
{
    bool dataStored = false;
    m_averager->newData(data);
    if (m_averager->full())
    {
        prePush();
        m_data[getWriteIndex()] = (float)m_averager->getFloatAverage();
        postPush();
        m_averager->reset(NULL);
        dataStored = true;
    }
    return dataStored;
}

void NumericDataField::getRawDataAsString(char * buf, char const * const fmt, bool alsoRemove)
{
    float data = getRawData(alsoRemove);
    sprintf(buf, fmt, data); // Write data point to buffer
}

void NumericDataField::getConvDataAsString(char * buf, char const * const fmt, bool alsoRemove)
{
    float data = getConvData(alsoRemove);
    sprintf(buf, fmt, data); // Write data point to buffer
}


void NumericDataField::getConfigString(char * buffer)
{
    if (!buffer) { return; }

    if (m_conversionData)
    {
        switch (m_fieldType)
        {
        case VOLTAGE:
            printVoltageData(buffer, (VOLTAGECHANNEL*)m_conversionData);
            break;
        case CURRENT:
            printCurrentData(buffer, (CURRENTCHANNEL*)m_conversionData);
            break;
        case TEMPERATURE_C:
            printThermistorData(buffer, (THERMISTORCHANNEL*)m_conversionData);
        default:
            break;
        }
    }
    else
    {
        sprintf(buffer, "No conversion data");
    }
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