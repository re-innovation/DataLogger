/*
 * Standard library includes
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <math.h>

/*
 * Utility Library Includes
 */

/*
 * Device Library Includes
 */

#include "DLSensor.Thermistor.h"

/*
 * Private defines and typedefs
 */

/*
 * Private Variables
 */

/*
 * Private Function Prototypes
 */

/*
 * Public Functions
 */

static const float T25CinKelvin = 298.15f;
static const float T0CinKelvin = 273.15f;

Thermistor::Thermistor(float B, float R25, bool highSide)
{
	m_B = B;
	m_R25 = R25;
	m_Rinf = R25 * exp(-B / T25CinKelvin);
	m_highSide = highSide;
}

float Thermistor::TemperatureFromResistance(float R)
{
	float t;

	t = m_B / log(R / m_Rinf);
	
	return t - T0CinKelvin;
}

float Thermistor::TemperatureFromADCReading(float otherResistor, uint16_t reading, uint16_t maxReading)
{
	return TemperatureFromADCReading(otherResistor, (float)reading, maxReading);
}

float Thermistor::TemperatureFromADCReading(float otherResistor, float reading, uint16_t maxReading)
{
	float result  = 0;
	float myResistance;

	if (m_highSide)
	{
		myResistance = (float)maxReading / reading;
		myResistance -= 1;
		myResistance *= otherResistor;
	}
	else
	{
		myResistance = otherResistor * reading;
		myResistance /= ((float)maxReading - reading);
	}

	if (myResistance > 0)
	{
		result = TemperatureFromResistance(myResistance);
	}
	
	return result;
}

float Thermistor::ResistanceFromTemperature(float t)
{
	t = t + T0CinKelvin;
	
	float res = exp(m_B / t) * m_Rinf;
	
	return res;
}

float Thermistor::Rinf(void)
{
	return m_Rinf;
}