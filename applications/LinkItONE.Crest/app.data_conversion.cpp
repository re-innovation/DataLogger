/*
 * app.data_conversion.cpp
 *
 * James Fowkes
 *
 * 03 April 2015
 *
 * Performs conversion from raw ADC readings to actual voltages and currents
 */

#include <stdint.h>

#include "DLSensor.Thermistor.h"
#include "app.data_conversion.h"

static Thermistor s_thermistors[] = {
    Thermistor(3977, 10000),
    Thermistor(3977, 10000),
    Thermistor(3977, 10000)
};

float adcTomV(uint16_t in, float mvPerBit)
{
    return (float)in * mvPerBit;
}

/* Assuming that the voltage in is from a potential divider
of resistors r1 and r2, calculate the divider input voltage */
float potentialDividerConversion(float in, float r1, float r2)
{
	return (in * (r1 + r2)) / r2;
}

/* Where a reading in mV represents a current,
perform a conversion based on mvPerAmp factor.
Since sensors may output at >0V for 0A, allow application of an offset */
float mvToAmps(float mV, float mVperAmp, float mvAtZero)
{
	return (mV - mvAtZero) / mVperAmp;
}

float channel01Conversion(uint16_t in)
{
	float mv = adcTomV(in, 0.125);
	return mv;//potentialDividerConversion(mv, 200000.0, 10000.0);
}

float channel02Conversion(uint16_t in)
{
	float mv = adcTomV(in, 0.125);
	return mv;//potentialDividerConversion(mv, 200000.0, 10000.0);
}

float channel03Conversion(uint16_t in)
{
	float mv = adcTomV(in, 0.125);
	return mv;//potentialDividerConversion(mv, 200000.0, 10000.0);
}

float channel04Conversion(uint16_t in)
{
	float mv = adcTomV(in, 0.125);
	return mv;//potentialDividerConversion(mv, 200000.0, 10000.0);
}

float channel05Conversion(uint16_t in)
{
	float mv = adcTomV(in, 0.125);
	return mvToAmps(mv, 59, 594);
}

float channel06Conversion(uint16_t in)
{
	float mv = adcTomV(in, 0.125);
	return mvToAmps(mv, 59, 592);
}

float channel07Conversion(uint16_t in)
{
	float mv = adcTomV(in, 0.125);
	return mvToAmps(mv, 59, 591);
}

float channel08Conversion(uint16_t in)
{
	float mv = adcTomV(in, 0.125);
	return mvToAmps(mv, 60, 596);
}

float channel09Conversion(uint16_t in)
{
	float mv = adcTomV(in, 0.125);
	return mvToAmps(mv, 59, 592);
}

float channel10Conversion(uint16_t in)
{
	float mv = adcTomV(in, 0.125);
	return mvToAmps(mv, 59, 592);
}

float channel11Conversion(uint16_t in)
{
	float mv = adcTomV(in, 0.125);
	return potentialDividerConversion(mv, 200000.0, 10000.0);
}

float channel12Conversion(uint16_t in)
{
	float mv = adcTomV(in, 0.125);
	return potentialDividerConversion(mv, 200000.0, 1000.0);
}

float temp1Conversion(uint16_t in)
{
	return s_thermistors[0].TemperatureFromADCReading(10000.0, in, 675);
}

float temp2Conversion(uint16_t in)
{
	return s_thermistors[1].TemperatureFromADCReading(10000.0, in, 675);
}

float temp3Conversion(uint16_t in)
{
	return s_thermistors[2].TemperatureFromADCReading(10000.0, in, 675);
}
