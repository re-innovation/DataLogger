/*
 * data_field.cpp
 * 
 * Stores single item of data and provides get/set functoinality
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

/*
 * Local Application Includes
 */

#include "DLDataField.h"

/*
 * Public Functions 
 */

DataField::DataField(FIELD_TYPE fieldType)
{
 	m_fieldType = fieldType;
 	m_data = 0.0f;
}

DataField::~DataField() {}

FIELD_TYPE DataField::GetType(void)
{
	return m_fieldType;
}

template <typename T>
void DataField::StoreData(T data)
{
	// Data is always stored internally as a float (except for strings)
	// See bottom of file for allowable types for T
	m_data = (float)data;
}

void DataField::StoreData(char * str) { StoreData((char const * const)str); }

void DataField::StoreData(char const * const str)
{
	// Strings are copied into the local buffer
	strncpy(m_stringData, str, MAX_DATAFIELD_STRING_SIZE);
}

void DataField::GetDataAsString(char * buf, char const * const fmt)
{
	if (m_fieldType == GENERIC_STRING)
	{
		// Strings are a special case where format is not used
		strncpy(buf, m_stringData, MAX_DATAFIELD_STRING_SIZE);
	}
	else
	{
		sprintf(buf, fmt, m_data);
	}
}

float DataField::GetDataAsFloat(void)
{
	return (m_fieldType != GENERIC_STRING) ? m_data : 0.0f;
}

// Explictly instantiate templates for StoreData
template void DataField::StoreData<uint8_t>(uint8_t);
template void DataField::StoreData<int8_t>(int8_t);
template void DataField::StoreData<uint16_t>(uint16_t);
template void DataField::StoreData<int16_t>(int16_t);
template void DataField::StoreData<uint32_t>(uint32_t);
template void DataField::StoreData<int32_t>(int32_t);
template void DataField::StoreData<float>(float);
