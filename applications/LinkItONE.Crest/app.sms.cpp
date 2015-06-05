/*
 * app.sms.cpp
 *
 * James Fowkes
 *
 * 06 April 2015
 *
 * Handles SMS messaging for CREST application
 */

/*
 * Arduino Library Includes
 */

#include <arduino.h>

/*
 * Standard Library Includes
 */

#include <stdint.h>

 /*
 * DataLogger Includes
 */

#include "DLSettings.h"
#include "DLSettings.Global.h"
#include "DLSMS.h"

/*
 * Application Includes
 */

#include "app.sms.h"

static SMSInterface * s_linkitoneSMSInterface;

bool APP_SMS_Setup(void)
{
	s_linkitoneSMSInterface = SMS_GetInterface(SMS_INTERFACE_LINKITONE);
}

void APP_SMS_SendGeneralMessage(char * message)
{
	if (!message) { return; }

	/* There can be up to 4 phone numbers set in settings */
	if (Settings_stringIsSet(GENERAL_PHONE_NUMBER_1))
	{
		s_linkitoneSMSInterface->send(Settings_getString(GENERAL_PHONE_NUMBER_1), message);
	}
	
	if (Settings_stringIsSet(GENERAL_PHONE_NUMBER_2))
	{
		s_linkitoneSMSInterface->send(Settings_getString(GENERAL_PHONE_NUMBER_2), message);
	}
	
	if (Settings_stringIsSet(GENERAL_PHONE_NUMBER_3))
	{
		s_linkitoneSMSInterface->send(Settings_getString(GENERAL_PHONE_NUMBER_3), message);
	}

	if (Settings_stringIsSet(GENERAL_PHONE_NUMBER_4))
	{
		s_linkitoneSMSInterface->send(Settings_getString(GENERAL_PHONE_NUMBER_4), message);
	}	
}

void APP_SMS_SendMaintenanceMessage(char * message)
{
	if (!message) { return; }

	/* There can be up to 4 phone numbers set in settings */
	if (Settings_stringIsSet(MAINTENANCE_PHONE_NUMBER_1))
	{
		s_linkitoneSMSInterface->send(Settings_getString(MAINTENANCE_PHONE_NUMBER_1), message);
	}
	
	if (Settings_stringIsSet(MAINTENANCE_PHONE_NUMBER_2))
	{
		s_linkitoneSMSInterface->send(Settings_getString(MAINTENANCE_PHONE_NUMBER_2), message);
	}
	
	if (Settings_stringIsSet(MAINTENANCE_PHONE_NUMBER_3))
	{
		s_linkitoneSMSInterface->send(Settings_getString(MAINTENANCE_PHONE_NUMBER_3), message);
	}

	if (Settings_stringIsSet(MAINTENANCE_PHONE_NUMBER_4))
	{
		s_linkitoneSMSInterface->send(Settings_getString(MAINTENANCE_PHONE_NUMBER_4), message);
	}	
}