/*
 * DLSMS.cpp
 * 
 * Interface for sending SMS messages
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
#include <stdlib.h>
#endif

/*
 * Local Application Includes
 */

#include "DLSMS.h"
#include "DLSMS.linkitone.h"

/*
 * Private Variables
 */

/*
 * Private Functions 
 */
 
/*
 * Public Functions 
 */

SMSInterface * SMS_GetInterface(SMS_INTERFACE provider)
{
	switch(provider)
	{
	case SMS_INTERFACE_LINKITONE:
		return new LinkItOneSMSInterface();
	}
}