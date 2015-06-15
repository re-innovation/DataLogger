/*
 * DLPlatform.LinkItONE.cpp
 *
 * Provides special handling for the LinkItONE platform
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
#endif

/*
 * Local Application Includes
 */

#include "DLUtility.PD.h" 
#include "DLUtility.Averager.h"

#include "DLDataField.Types.h"
#include "DLDataField.Conversion.h"
#include "DLDataField.h"

#include "DLSensor.Thermistor.h"

#include "DLPlatform.h"

/*
 * Compenstates thermsitor readings for 33K+33K divider on
 * ADC inputs
 */

static float thermistorADCConversion(float in, void * field)
{
	if (!field) { return in; }

	THERMISTORCHANNEL * pChannelParams = (THERMISTORCHANNEL*)field;

	// Get the thermistor voltage
	float mv = CONV_ADCtoMillivolts(in, 5000.0f/1024.0f);

	// Recalculate actual thermistor resistance based on divider and 66K in parallel
	float parallelR = PD_GetLowerResistance(pChannelParams->otherR, 3300, mv);
	float thermistorR = 1.0/((1.0/parallelR) - (1.0/66000.0));
	
	Thermistor thermistor = Thermistor(pChannelParams->B, pChannelParams->R25, pChannelParams->highside);
	return thermistor.TemperatureFromResistance(thermistorR);
}


void PLATFORM_LINKITONE_specialFieldSetup(NumericDataField * field)
{
	if (!field) { return; }

	// Thermistors on channels 13, 14, 15 need special handling
	// due to the 33k+33K potential divider on those ADC inputs.

	if (field->getChannelNumber() <= 12) { return; }

	if (field->getType() == TEMPERATURE_C)
	{
		field->setAltConversion(thermistorADCConversion);
	}
}