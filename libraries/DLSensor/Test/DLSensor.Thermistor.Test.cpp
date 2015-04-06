#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "unity.h"

#include "../DLSensor.Thermistor.h"

#define RESISTANCE_AT_25C 10000UL
#define BETA 3000U

Thermistor t(3000U, RESISTANCE_AT_25C);

uint32_t rHiTemp = 4590; // Calculated empirically for a 3000 beta thermistor at +50C
uint32_t rLoTemp = 97500; // Calculated empirically for a 3000 beta thermistor at -30C

char messageBuffer[256];

void test_ValidAt25Degrees(void)
{
	float temp = t.TemperatureFromResistance(RESISTANCE_AT_25C);
	TEST_ASSERT_EQUAL(25, temp);

	float r = t.ResistanceFromTemperature(25);
	
	// Readings should be within 1% of expected
	TEST_ASSERT_UINT_WITHIN((RESISTANCE_AT_25C+50)/100, RESISTANCE_AT_25C, r);
	
	temp = t.TemperatureFromResistance(r);
	TEST_ASSERT_EQUAL(25, temp);
}

void test_ValidBetweenMinus30And50(void)
{
	uint32_t r;
	float temp;
	// Test valid conversions between -30 and 50 degrees celcius

	for (r = rHiTemp; r < rLoTemp; ++r)
	{
		temp = t.TemperatureFromResistance(r);
		sprintf(messageBuffer, "R = %u, Rinf = %f, t=%f (MaxR=%u, MinR=%u)", r, t.Rinf(), temp, rLoTemp, rHiTemp);
				
		TEST_ASSERT_MESSAGE(temp <= 51, messageBuffer);
		TEST_ASSERT_MESSAGE(temp >= -31, messageBuffer);
	}
}

int main(void)
{
	UnityBegin("DLSensor.Thermistor.Test.cpp");

	RUN_TEST(test_ValidAt25Degrees);
	RUN_TEST(test_ValidBetweenMinus30And50);

	return (UnityEnd());
}
