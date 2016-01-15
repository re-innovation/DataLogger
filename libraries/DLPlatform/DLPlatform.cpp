/*
 * DLPlatform.cpp
 *
 * Provides special handling for different platforms
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

void PLATFORM_specialFieldSetup(NumericDataField * field)
{
	if (!field) { return; }

	#ifdef __LINKIT_ONE__
	PLATFORM_LINKITONE_specialFieldSetup(field);
	#endif
}