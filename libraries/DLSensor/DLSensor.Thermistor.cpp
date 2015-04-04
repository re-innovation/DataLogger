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

	//Rinf = R25 * exp(-B/T0) = R25 / exp(B/T0)
	m_Rinf = m_B / T25CinKelvin;
	m_Rinf = exp(m_Rinf);	
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

/*uint32_t THERMISTOR_GetResistanceFromTemperature(THERMISTOR * pTherm, FIXED_POINT_TYPE t)
{
	// Convert celcius to kelvin
	t = fp_add(t, T0CinKelvin);
	
	FIXED_POINT_TYPE r = fp_div(pTherm->B, t);
	uint32_t res;
	
	#if USE_FIX16
	r = fp_div(r, fp_from_int(2)); // Division by 2 to keep value in 16.16 range during exponentiation
	r = fp_exp(r);
	
	// Use Rinf x100 to get enough significant figures
	uint32_t Rinf100 = (uint32_t)fp_to_int(fp_mul(pTherm->Rinf, fp_from_int(100)));
	
	res = (uint32_t)fp_to_int(r);
	res = res * res; // Square to undo division by 2
	res *= Rinf100;
	res = (res + 50) / 100; // Undo multiplication by 100
	
	#else
	res = fp_exp(r) * pTherm->Rinf;
	#endif
	
	return res;
}*/
