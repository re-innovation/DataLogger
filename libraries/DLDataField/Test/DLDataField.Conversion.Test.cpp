/*
 * DLDataField.Conversion.Test.cpp
 * 
 * Tests the DataField class
 *
 * Author: James Fowkes
 *
 * www.re-innovation.co.uk
 */

/*
 * C++ Library Includes
 */

#include <stdint.h>
#include <string.h>

#include <iostream>

/*
 * Unity Test Framework
 */

#include "unity.h"


/*
 * Local Application Includes
 */

#include "DLDataField.Types.h"
#include "DLDataField.Conversion.h"

void test_ConversionToVoltsIsCorrect(void)
{
	VOLTAGECHANNEL testChannel = 
	{
	    .mvPerBit = 0.125,
	    .R1 = 10000,
	    .R2 = 10000
	};

	float expected = 0.0f;
	float actual = CONV_VoltsFromRaw(0, &testChannel);
	TEST_ASSERT_EQUAL_FLOAT(expected, actual);

	expected = 0.25575f;
	actual = CONV_VoltsFromRaw(1023, &testChannel);
	TEST_ASSERT_EQUAL_FLOAT(expected, actual);

	expected = 1.0f;
	actual = CONV_VoltsFromRaw(4000, &testChannel);
	TEST_ASSERT_EQUAL_FLOAT(expected, actual);
}

void test_ConversionToAmpsIsCorrect(void)
{
	CURRENTCHANNEL testChannel = {
    	.mvPerBit = 0.125,
    	.offset = 600,
    	.mvPerAmp = 60
    };

	float expected = 0.0f;
	float actual = CONV_AmpsFromRaw(4800, &testChannel);
	TEST_ASSERT_EQUAL_FLOAT(expected, actual);

	expected = 1.0f;
	actual = CONV_AmpsFromRaw(5280, &testChannel);
	TEST_ASSERT_EQUAL_FLOAT(expected, actual);
};

void test_ConversionHighsideThermistorToTemperatureIsCorrect(void)
{
	THERMISTORCHANNEL testChannel = 
	{
    	.R25 = 10000,
    	.B = 3000,
    	.otherR = 10000,
    	.maxADC = 1023,
    	.highside = true
	};

	char message[128];
	float expected = 25.0f;
	float actual = CONV_CelsiusFromRawThermistor(511.5f, &testChannel);
	sprintf(message, "Expected %.3f, actual %.3f", expected, actual);

	TEST_ASSERT_EQUAL_FLOAT_MESSAGE(expected, actual, message);

	/* Emprically determined values from ADC readings
	(see DLSensor.Thermistor.Test.cpp) */
	expected = 6.36;
	actual = CONV_CelsiusFromRawThermistor(346.0f, &testChannel);
	sprintf(message, "Expected %.3f, actual %.3f", expected, actual);

	TEST_ASSERT_FLOAT_WITHIN_MESSAGE(expected, actual, expected/100, message);

	expected = 51.74;
	actual = CONV_CelsiusFromRawThermistor(712.0f, &testChannel);
	sprintf(message, "Expected %.3f, actual %.3f", expected, actual);

	TEST_ASSERT_FLOAT_WITHIN_MESSAGE(expected, actual, expected/100, message);

	expected = 79.59;
	actual = CONV_CelsiusFromRawThermistor(845.0f, &testChannel);
	sprintf(message, "Expected %.3f, actual %.3f", expected, actual);

	TEST_ASSERT_FLOAT_WITHIN_MESSAGE(expected, actual, expected/100, message);
}

int main(void)
{
    UnityBegin("DLDataField.Conversion.cpp");

    RUN_TEST(test_ConversionToVoltsIsCorrect);
    RUN_TEST(test_ConversionToAmpsIsCorrect);
    RUN_TEST(test_ConversionHighsideThermistorToTemperatureIsCorrect);

    UnityEnd();
    return 0;
}
