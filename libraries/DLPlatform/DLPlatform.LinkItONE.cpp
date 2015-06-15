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
 
#include "DLUtility.Averager.h"
#include "DLDataField.Types.h"
#include "DLDataField.h"

#include "DLPlatform.h"

/*
 * Compenstates thermsitor readings for 33K+33K divider on
 * ADC inputs
 */

static float thermistorADCConversion(float in, void * field)
{
	if (!field) { return in; }

	THERMISTORCHANNEL * pChannelParams = (THERMISTORCHANNEL*)field;

	if (!pChannelParams) { return in; }

	return in;
}

void PLATFORM_LINKITONE_specialFieldSetup(NumericDataField * field)
{
	if (!field) { return; }

	// Thermistors on channels 13, 14, 15 need special handling
	// due to the 33k+33K potential divider on those ADC inputs.

	if (field->getChannelNumber() <= 12) { return; }

	if (field->getType() == TEMPERATURE_C)
	{
		field->setExtraConversion(thermistorADCConversion);
	}
}