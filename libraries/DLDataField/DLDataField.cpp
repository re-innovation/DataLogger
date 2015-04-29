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
#include "DLDataField.Types.h"
#include "DLDataField.h"
#include "DLUtility.h"

/*
 * Defines and Typedefs
 */

// The had/tail indexes for data arrays are stored in a two-value array.
#define H 0 // First entry is the head of the list
#define T 1 // Second entry is the tail

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

DataField::DataField(FIELD_TYPE fieldType, uint32_t channelNumber)
{
 	m_fieldType = fieldType;
 	m_index[T] = 0;
    m_index[H] = 0;
    m_maxIndex = 0;
    m_channelNumber = channelNumber;
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

void DataField::pop(void)
{
	if (length())
	{
		m_index[T]++;
	}
}

void DataField::prePush(void)
{
	if (full()) { removeOldest(); }
}

void DataField::postPush(void)
{
	m_index[H]++;
}

bool DataField::full(void)
{
	return (m_index[H] - m_index[T]) >= (m_maxIndex + 1);
}

/*void DataField::incrementIndexes(void)
{
	incrementwithrollover(m_index[H], m_maxIndex);
	if (m_index[H] == 0) { full() = true; } // Write index has rolled over, so buffer is full

	if (full())
	{
		// When full, the write index points to the oldest value (where data should be read from)
		m_index[T] = m_index[H];
	}
}*/

uint32_t DataField::getWriteIndex(void)
{
	return m_index[H] % (m_maxIndex + 1);
}

uint32_t DataField::getTailIndex(void)
{
	return m_index[T] % (m_maxIndex + 1);
}

/*uint32_t DataField::getRealReadIndex(uint32_t requestedIndex)
{
	//Translate request for index based on 0 being oldest stored value
	//into index based on circular array storage
	requestedIndex += m_index[T];
	requestedIndex %= (m_maxIndex + 1);
	return requestedIndex;
}*/

char const * DataField::getTypeString(void)
{
	return _getTypeString(m_fieldType);
}

uint32_t DataField::length(void)
{
		return min((m_index[H] - m_index[T]), m_maxIndex + 1);
}

void DataField::removeOldest(void)
{
	// Move the read index on one, effectively ignoring the oldest value.
	if (length() > 0)
	{
		m_index[T]++;
	}
}

bool DataField::hasData(void)
{
	return length() > 0;
}

uint32_t DataField::getChannelNumber(void)
{
	return m_channelNumber;
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