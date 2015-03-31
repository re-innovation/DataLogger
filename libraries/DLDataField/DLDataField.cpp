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
#include "DLUtility.h"

/*
 * Public Functions 
 */

DataField::DataField(FIELD_TYPE fieldType)
{
 	m_fieldType = fieldType;
}

DataField::~DataField() {}

FIELD_TYPE DataField::getType(void)
{
	return m_fieldType;
}

template <typename T>
NumericDataField<T>::NumericDataField(FIELD_TYPE type, uint8_t N) : DataField(type)
{
	m_data = new T[N];
	
	if (m_data)
	{
		fillArray(m_data, (T)0, N);
	}

	m_index = 0;
	m_maxIndex = N;
}

template <typename T>
NumericDataField<T>::~NumericDataField()
{
	delete[] m_data;
}

template <typename T>
T NumericDataField<T>::getData(uint8_t index)
{
	return (index < m_maxIndex) ? m_data[index] : 0;
}

template <typename T>
void NumericDataField<T>::storeData(T data)
{
	m_data[m_index] = (float)data;
	incrementwithrollover(m_index, m_maxIndex);
}

template <typename T>
void NumericDataField<T>::getDataAsString(char * buf, char const * const fmt, uint8_t index)
{
	sprintf(buf, fmt, m_data[index]); // Write data point to buffer
}

StringDataField::StringDataField(FIELD_TYPE type, uint8_t len, uint8_t N) : DataField(type)
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

	m_index = 0;
	m_maxIndex = N;
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

void StringDataField::storeData(char * data)
{
	strncpy(m_data[m_index], data, m_maxLength);
	incrementwithrollover(m_index, m_maxIndex);
}

char * StringDataField::getData(uint8_t index)
{
	return (index < m_maxIndex) ? m_data[index] : NULL; 
}

void StringDataField::copy(char * buf, uint8_t index)
{
	if (index < m_maxIndex)
	{
		strncpy(buf, m_data[index], m_maxLength);
	}
}

// Explictly instantiate templates for NumericDataField
template class NumericDataField<uint8_t>;
template class NumericDataField<int8_t>;
template class NumericDataField<uint16_t>;
template class NumericDataField<int16_t>;
template class NumericDataField<uint32_t>;
template class NumericDataField<int32_t>;
template class NumericDataField<float>;
