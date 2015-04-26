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

#include "DLUtility.Averager.h"
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

/*
 * DataField Class Functions 
 */

DataField::DataField(FIELD_TYPE fieldType)
{
 	m_fieldType = fieldType;
 	m_full = false;
 	m_index[R] = 0;
    m_index[W] = 0;
    m_maxIndex = 0;
}

DataField::~DataField() {}

void DataField::setSize(uint32_t length)
{
 	m_maxIndex = length - 1;
}

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

uint32_t DataField::getWriteIndex(void)
{
	return m_index[W];
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