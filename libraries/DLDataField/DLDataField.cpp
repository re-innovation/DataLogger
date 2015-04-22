/*
 * DLDataField.cpp
 * 
 * Stores single item of data and provides get/set functionality
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

/*
 * Defines and Typedefs
 */

// The read/write indexes for data arrays are stored in a two-value array.
#define R 0 // First entry is the read index
#define W 1 // Second entry is the write index

/*
 * Private Functions 
 */

static char const * _getTypeString(FIELD_TYPE type)
{

	static char const * fieldtypestrings[] = {
    	"Voltage (V)", // VOLTAGE
    	"Current (A)", // CURRENT

    	"Temp (C)", // TEMPERATURE_C
    	"Temp (F)", // TEMPERATURE_F
    	"Temp (K)", // TEMPERATURE_K
    
    	"Irradiance (W/m2)", // IRRADIANCE_WpM2

    	"Wind Direction", // CARDINAL_DIRECTION
    	"Wind Direction" // DEGREES_DIRECTION		
	};

	return (type <= DEGREES_DIRECTION) ? fieldtypestrings[type] : "";
	
}

static uint32_t writeHeadersToBuffer(
	char * buffer, DataField * datafields[], uint8_t arrayLength, uint8_t bufferLength)
{
	if (!buffer) { return 0; }

	uint8_t i;
	
	FixedLengthAccumulator headerAccumulator(buffer, bufferLength);

	for (i = 0; i < arrayLength; ++i)
	{
		headerAccumulator.writeString(datafields[i]->getTypeString());
		if (!lastinloop(i, arrayLength))
		{
			headerAccumulator.writeString(", ");
		}
	}

	headerAccumulator.writeString("\r\n");

	return headerAccumulator.length();
}

/*
 * DataField Class Functions 
 */

DataField::DataField(FIELD_TYPE fieldType, uint32_t length)
{
 	m_fieldType = fieldType;
 	m_full = false;
 	m_maxIndex = length - 1;
}

DataField::~DataField() {}

FIELD_TYPE DataField::getType(void)
{
	return m_fieldType;
}

void DataField::incrementIndexes(void)
{
	incrementwithrollover(m_index[W], m_maxIndex);
	if (m_index[W] == 0) { m_full = true; } // Write index has rolled over, so buffer is full

	if (m_full)
	{
		// When full, the write index points to the oldest value (where data should be read from)
		m_index[R] = m_index[W];
	}
}

uint32_t DataField::getRealReadIndex(uint32_t requestedIndex)
{
	/* Translate request for index based on 0 being oldest stored value
	into index based on circular array storage */
	requestedIndex += m_index[R];
	requestedIndex %= (m_maxIndex + 1);
	return requestedIndex;
}

char const * DataField::getTypeString(void)
{
	return _getTypeString(m_fieldType);
}

NumericDataField::NumericDataField(FIELD_TYPE type, uint32_t N) : DataField(type, N)
{
	m_data = new float[N];
	
	if (m_data)
	{
		fillArray(m_data, 0.0f, N);
	}

	m_index[R] = 0;
	m_index[W] = 0;
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
	m_data[m_index[W]] = (float)data;
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

StringDataField::StringDataField(FIELD_TYPE type, uint8_t len, uint32_t N) : DataField(type, N)
{
	m_data = new char*[N];
	
	if (m_data)
	{
		uint8_t i = 0;
		for (i = 0; i < N; i++)
		{
			m_data[i] = new char[len];
		}
	}

	m_index[R] = 0;
	m_index[W] = 0;
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
	strncpy_safe(m_data[m_index[W]], data, m_maxLength);
	incrementIndexes();
}

char * StringDataField::getData(uint32_t index)
{
	index = getRealReadIndex(index);
	return m_data[index];
}

void StringDataField::copy(char * buf, uint32_t index)
{
	index = getRealReadIndex(index);
	strncpy_safe(buf, m_data[index], m_maxLength);
}

uint32_t DataField_writeHeadersToBuffer(
	char * buffer, DataField * datafields[], uint8_t arrayLength, uint8_t bufferLength)
{
	// Just pass straight to private function
	return writeHeadersToBuffer(buffer, datafields, arrayLength, bufferLength);
}

uint32_t DataField_writeHeadersToBuffer(
	char * buffer, DataField datafields[], uint8_t arrayLength, uint8_t bufferLength)
{
	// Build array of pointers and pass to private function 
	DataField * pDataFields[arrayLength];

	uint8_t i;
	for (i = 0; i < arrayLength; i++)
	{
		pDataFields[i] = &datafields[i];
	}

	return writeHeadersToBuffer(buffer, pDataFields, arrayLength, bufferLength);
}

/* In-progress functions
uint32_t DataField_writeNumericDataToBuffer(
	char * buffer, NumericDataField datafields[], char const * const format, uint8_t arrayLength, uint8_t bufferLength)
{
	if (!buffer) { return 0; }

	uint8_t i;
	char numericBuffer[10];

	FixedLengthAccumulator headerAccumulator(buffer, bufferLength);

	for (i = 0; i < arrayLength; ++i)
	{
		datafields[i].getDataAsString(numericBuffer, format, 0);
		headerAccumulator.writeString(numericBuffer);		
		if (!lastinloop(i, arrayLength))
		{
			headerAccumulator.writeString(", ");
		}
	}

	return headerAccumulator.length();
}

uint32_t DataField_writeStringDataToBuffer(
	char * buffer, StringDataField datafields[], uint8_t arrayLength, uint8_t bufferLength)
{
if (!buffer) { return 0; }

	uint8_t i;

	FixedLengthAccumulator headerAccumulator(buffer, bufferLength);

	for (i = 0; i < arrayLength; ++i)
	{
		headerAccumulator.writeString(datafields[i].getData(0));		
		if (!lastinloop(i, arrayLength))
		{
			headerAccumulator.writeString(", ");
		}
	}

	return headerAccumulator.length();	
}*/

/* In-progress functions 
template uint32_t DataField_writeNumericDataToBuffer(
	char * buffer, NumericDataField datafields[], char const * const format, uint8_t arrayLength, uint8_t bufferLength);
*/