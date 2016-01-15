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
#include "DLUtility.PD.h"
#include "DLSensor.Thermistor.h"
#include "DLDataField.Types.h"
#include "DLDataField.h"

/*
 * Private Functions
 */

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
 * CONV_ADCtoMillivolts
 *
 * Convert a raw ADC reading to millivolts
 *
 * in: Raw ADC reading
 * mvPerBit: Millivolts per ADC bit
 */
float CONV_ADCtoMillivolts(float in, float mvPerBit)
{
	return (in * mvPerBit);
}

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
		float volts = CONV_ADCtoMillivolts(raw, conversionData->mvPerBit) / 1000;
    	volts -= conversionData->offset;
    	volts *= conversionData->multiplier;
    	return PD_GetInputVoltage(volts, conversionData->R1, conversionData->R2);
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
		float mv = CONV_ADCtoMillivolts(raw, conversionData->mvPerBit);
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
