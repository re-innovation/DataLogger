/*
 * DLUtility.Averager.cpp
 * 
 * Provides averaging functionality
 *
 * Author: James Fowkes
 *
 * www.re-innovation.co.uk
 */

/*
 * Standard Library Includes
 */
 
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
 
/*
 * Generic Library Includes
 */

#include "DLUtility.Averager.h"
#include "DLUtility.HelperMacros.h"
#include "DLUtility.ArrayFunctions.h"

/*
 * Defines and Typedefs
 */

/*
 * Averager Class Definition
 */

template <typename T>
Averager<T>::Averager(uint16_t size)
{
	m_data = new T[size];
	m_write = 0;
	m_maxIndex = size -1;
	m_full = false;
}

template <typename T>
uint16_t Averager<T>::size(void)
{
	return m_maxIndex + 1;
}

template <typename T>
void Averager<T>::reset(T * value)
{
	/* The value is passed as a pointer so that NULL can
	represent resetting to zero length (rather than resetting to full of zeroes) */
	if (!value)
	{
		fillArray(m_data, (T)0, size());
	}
	else
	{
		fillArray(m_data, *value, size());
	}

	m_write = 0;
	m_full = (value != NULL); 
}

template <typename T>
uint16_t Averager<T>::N(void)
{
	return m_full ? m_maxIndex : m_write - 1;
}

template <typename T>
T Averager<T>::getAverage(void)
{
	int64_t sum = 0;
	uint16_t count = 0;

	if (m_write || m_full)
	{
		uint16_t n = 0;
		count = m_full ? m_maxIndex : m_write - 1;
		for (n = 0; n <= count; n++)
		{
			sum += m_data[n];
		}
	}
	else
	{
		sum = 0;
	}

	sum = div_round(sum, count+1);
	return sum;
}

template <typename T>
void Averager<T>::newData(T newData)
{
	m_data[m_write] = newData;
	m_full |= (m_write == m_maxIndex);
	incrementwithrollover(m_write, m_maxIndex);
}

#ifdef TEST
template <typename T>
void Averager<T>::fillFromArray(T * array, uint16_t size)
{
	uint16_t i;
	for (i = 0; i < size; ++i)
	{
		newData(array[i]);
	}
}
#endif

template class Averager<float>;
template class Averager<uint8_t>;
template class Averager<int8_t>;
template class Averager<uint16_t>;
template class Averager<int16_t>;
template class Averager<uint32_t>;
template class Averager<int32_t>;
