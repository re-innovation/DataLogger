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
static const float _log2 = 0.69314718055994530941723212145818f;

Thermistor::Thermistor(float B, float R25)
{
	m_B = B;
	m_R25 = R25;

	//Rinf = R25 * exp(-B/T0)
	m_Rinf = -m_B / T25CinKelvin;
	m_Rinf = exp(m_Rinf);	
	m_Rinf *= R25;
}

float Thermistor::TemperatureFromResistance(float R)
{
	float t;

	t = R / m_Rinf;
	t = m_B / log(t);
	
	return t - T0CinKelvin;
}

float Thermistor::TemperatureFromADCReading(float otherResistor, uint16_t reading)
{
	float result  = 0;
	
	float myResistance = otherResistor * (float)reading;
	myResistance /= (1023.0 - (float)reading);

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