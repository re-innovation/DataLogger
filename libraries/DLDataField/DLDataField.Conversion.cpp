/*
 * DLDataField.String.cpp
 * 
 * Stores single item of string data and provides get/set functionality
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
 * Datalogger Library Includes
 */

#include "DLUtility.Averager.h"
#include "DLSensor.Thermistor.h"
#include "DLDataField.Types.h"
#include "DLDataField.h"

/*
 * Private Functions
 */

/*
 * adcTomV
 *
 * Convert a raw ADC reading to millivolts
 *
 * in: Raw ADC reading
 * mvPerBit: Millivolts per ADC bit
 */
static float adcTomV(float in, float mvPerBit)
{
	return (in * mvPerBit);
}

/*
 * potentialDividerConversion
 *
 * Assuming that the voltage in is from a potential divider
 * of resistors r1 and r2, calculate the divider input voltage
 *
 * in: The input voltage
 * r1: The "top" resistor in the divider
 * r2: The "bottom" resistor in the divider
 */

static float potentialDividerConversion(float in, float r1, float r2)
{
	return (in * (r1 + r2)) / r2;
}

/* 
 * mvToAmps
 *
 * Where a reading in mV represents a current,
 * perform a conversion based on mvPerAmp factor.
 * Since sensors may output at >0V for 0A, allow application of an offset
 *
 * mv: millivolt reading representing current
 * mvPerAmp: conversion factor
 * mvAtZero : Zero offset
 */
static float mvToAmps(float mV, float mVperAmp, float mvAtZero)
{
	return (mV - mvAtZero) / mVperAmp;
}

/*
 * Public Functions
 */

/* 
 * CONV_VoltsFromRaw
 *
 * Convert a raw ADC reading into an input voltage, assuming the voltage
 * is from a potential divider and ADC are described by data in a VOLTAGECHANNEL.
 *
 * raw: Raw ADC reading
 * conversionData : Pointer to conversion data to use
 */
float CONV_VoltsFromRaw(float raw, VOLTAGECHANNEL * conversionData)
{
	if (conversionData)
	{
		float volts = adcTomV(raw, conversionData->mvPerBit) / 1000;
    	return potentialDividerConversion(volts, conversionData->R1, conversionData->R2);
    }
    else
    {
    	return raw;
    }
}

/* 
 * CONV_AmpsFromRaw
 *
 * Convert a raw ADC reading into a current, assuming the current and
 * ADC are described by data in a CURRENTCHANNEL.
 *
 * raw: Raw ADC reading
 * conversionData : Pointer to conversion data to use
 */
float CONV_AmpsFromRaw(float raw, CURRENTCHANNEL * conversionData)
{
	if (conversionData)
	{
		float mv = adcTomV(raw, conversionData->mvPerBit);
		return mvToAmps(mv,conversionData->mvPerAmp, conversionData->offset);
	}
	else
	{
		return raw;
	}
}

/* 
 * CONV_CelsiusFromRawThermistor
 *
 * Convert a raw ADC reading into a temperature, assuming the thermistor
 * is described by data in a THERMISTORCHANNEL
 * raw: Raw ADC reading
 * conversionData : Pointer to conversion data to use
 */
float CONV_CelsiusFromRawThermistor(float raw, THERMISTORCHANNEL * conversionData)
{
    Thermistor thermistor = Thermistor(conversionData->B, conversionData->R25, conversionData->highside);
    return thermistor.TemperatureFromADCReading(conversionData->otherR, raw, conversionData->maxADC);
}
