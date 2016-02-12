#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "unity.h"

#include "../DLSensor.Thermistor.h"

#define RESISTANCE_AT_25C 10000UL
#define BETA 3000U

Thermistor lowsideT(3000U, RESISTANCE_AT_25C, false);
Thermistor highsideT(3000U, RESISTANCE_AT_25C, true);

uint32_t rHiTemp = 4590; // Calculated empirically for a 3000 beta thermistor at +50C
uint32_t rLoTemp = 97500; // Calculated empirically for a 3000 beta thermistor at -30C

char messageBuffer[256];

void test_highsideValidAt25Degrees(void)
{
	float temp;
	float r;
	
	temp = highsideT.TemperatureFromResistance(RESISTANCE_AT_25C);
	TEST_ASSERT_EQUAL(25, temp);

	r = highsideT.ResistanceFromTemperature(25);
	TEST_ASSERT_UINT_WITHIN((RESISTANCE_AT_25C+50)/100, RESISTANCE_AT_25C, r); // Readings should be within 1% of expected
}

void test_lowsideValidAt25Degrees(void)
{
	float temp;
	float r;

	temp = lowsideT.TemperatureFromResistance(RESISTANCE_AT_25C);
	TEST_ASSERT_EQUAL(25, temp);

	r = lowsideT.ResistanceFromTemperature(25);
	TEST_ASSERT_UINT_WITHIN((RESISTANCE_AT_25C+50)/100, RESISTANCE_AT_25C, r); // Readings should be within 1% of expected
}

void test_highsideValidBetweenMinus30And50(void)
{
	uint32_t r;
	float temp;
	// Test valid conversions between -30 and 50 degrees celcius

	for (r = rHiTemp; r < rLoTemp; ++r)
	{
		temp = highsideT.TemperatureFromResistance(r);
		sprintf(messageBuffer, "R = %u, Rinf = %f, highsideT=%f (MaxR=%u, MinR=%u)", r, highsideT.Rinf(), temp, rLoTemp, rHiTemp);
				
		TEST_ASSERT_MESSAGE(temp <= 51, messageBuffer);
		TEST_ASSERT_MESSAGE(temp >= -31, messageBuffer);
	}
}

void test_lowsideValidBetweenMinus30And50(void)
{
	uint32_t r;
	float temp;
	// Test valid conversions between -30 and 50 degrees celcius

	for (r = rHiTemp; r < rLoTemp; ++r)
	{
		temp = lowsideT.TemperatureFromResistance(r);
		sprintf(messageBuffer, "R = %u, Rinf = %f, lowsideT=%f (MaxR=%u, MinR=%u)", r, lowsideT.Rinf(), temp, rLoTemp, rHiTemp);
				
		TEST_ASSERT_MESSAGE(temp <= 51, messageBuffer);
		TEST_ASSERT_MESSAGE(temp >= -31, messageBuffer);
	}
}

void test_EmpiricallyDeterminedValuesFromResistance(void)
{	
	uint32_t r;
	float tempHi, tempLo;
	float expected;

	// Test some empirically determined values
	tempHi = highsideT.TemperatureFromResistance(r = 5000);
	tempLo = highsideT.TemperatureFromResistance(r = 5000);
	expected = 47.06;
	sprintf(messageBuffer, "R = %u, highsideT=%f", r, tempHi);
	TEST_ASSERT_FLOAT_WITHIN_MESSAGE(expected, tempHi, expected/100, messageBuffer);
	TEST_ASSERT_EQUAL_FLOAT(tempHi, tempLo);

	tempHi = highsideT.TemperatureFromResistance(r = 15000);
	tempLo = highsideT.TemperatureFromResistance(r = 15000);
	expected = 13.45;
	sprintf(messageBuffer, "R = %u, highsideT=%f", r, tempHi);
	TEST_ASSERT_FLOAT_WITHIN_MESSAGE(expected, tempHi, expected/100, messageBuffer);
	TEST_ASSERT_EQUAL_FLOAT(tempHi, tempLo);

	tempHi = highsideT.TemperatureFromResistance(r = 12345);
	tempLo = highsideT.TemperatureFromResistance(r = 12345);
	expected = 18.89;
	sprintf(messageBuffer, "R = %u, highsideT=%f", r, tempHi);
	TEST_ASSERT_FLOAT_WITHIN_MESSAGE(expected, tempHi, expected/100, messageBuffer);
	TEST_ASSERT_EQUAL_FLOAT(tempHi, tempLo);
}

void test_highsideEmpiricallyDeterminedValuesFromADC(void)
{
	uint16_t adc;
	float temp;
	float expected;

	/* Test some empirically determined values.
	NTC resistance decreases with temperature
	Therefore in highside configuration
	temperature increases with voltage/ADC reads */

	temp = highsideT.TemperatureFromADCReading(10000.0, adc = 346, 1023);
	expected = 6.36;
	sprintf(messageBuffer, "adc = %u, highsideT=%f", adc, temp);
	TEST_ASSERT_FLOAT_WITHIN_MESSAGE(expected, temp, expected/100, messageBuffer);

	temp = highsideT.TemperatureFromADCReading(10000.0, adc = 712, 1023);
	expected = 51.74;
	sprintf(messageBuffer, "adc = %u, highsideT=%f", adc, temp);
	TEST_ASSERT_FLOAT_WITHIN_MESSAGE(expected, temp, expected/100, messageBuffer);

	temp = highsideT.TemperatureFromADCReading(10000.0, adc = 845, 1023);
	expected = 79.59;
	sprintf(messageBuffer, "adc = %u, highsideT=%f", adc, temp);
	TEST_ASSERT_FLOAT_WITHIN_MESSAGE(expected, temp, expected/100, messageBuffer);
}

void test_lowsideEmpiricallyDeterminedValuesFromADC(void)
{
	uint16_t adc;
	float temp;
	float expected;

	/* Test some empirically determined values.
	NTC resistance decreases with temperature
	Therefore in lowside configuration
	temperature decreases with voltage/ADC reads */

	temp = lowsideT.TemperatureFromADCReading(10000.0, adc = 346, 1023);
	expected = 46.31;
	sprintf(messageBuffer, "adc = %u, lowsideT=%f", adc, temp);
	TEST_ASSERT_FLOAT_WITHIN_MESSAGE(expected, temp, expected/100, messageBuffer);

	temp = lowsideT.TemperatureFromADCReading(10000.0, adc = 712, 1023);
	expected = 3.00;
	sprintf(messageBuffer, "adc = %u, lowsideT=%f", adc, temp);
	TEST_ASSERT_FLOAT_WITHIN_MESSAGE(expected, temp, expected/100, messageBuffer);

	temp = lowsideT.TemperatureFromADCReading(10000.0, adc = 845, 1023);
	expected = -14.97;
	sprintf(messageBuffer, "adc = %u, lowsideT=%f", adc, temp);
	TEST_ASSERT_FLOAT_WITHIN_MESSAGE(expected, temp, expected/100, messageBuffer);

	Thermistor oneHundredK(4400U, 100000, false);
	temp = oneHundredK.TemperatureFromADCReading(10000.0, adc = 596, 675);
	expected = 30.8;
	sprintf(messageBuffer, "adc = %u, lowsideT=%f", adc, temp);
	TEST_ASSERT_FLOAT_WITHIN_MESSAGE(expected, temp, expected/100, messageBuffer);
}

int main(void)
{
	UnityBegin("DLSensor.Thermistor.Test.cpp");

	RUN_TEST(test_highsideValidAt25Degrees);
	RUN_TEST(test_lowsideValidAt25Degrees);

	RUN_TEST(test_highsideValidBetweenMinus30And50);
	RUN_TEST(test_lowsideValidBetweenMinus30And50);

	RUN_TEST(test_EmpiricallyDeterminedValuesFromResistance);

	RUN_TEST(test_highsideEmpiricallyDeterminedValuesFromADC);
	RUN_TEST(test_lowsideEmpiricallyDeterminedValuesFromADC);

	return (UnityEnd());
}
