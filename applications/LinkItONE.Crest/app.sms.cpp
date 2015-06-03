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
static bool s_debugSMS = false;

static void sendMessage(STRINGSETTING numberID, char * message)
{
	char * number;
	bool success;
	if (Settings_stringIsSet(numberID))
	{
		number = Settings_getString(numberID);
		if (s_debugSMS) {Serial.print("Sending to "); Serial.print(number);}
		success = s_linkitoneSMSInterface->send(number, message);
		if (s_debugSMS) {Serial.print(success ? " succeeded" : " failed");}
	}
}

static void printNumber(STRINGSETTING numberID)
{
	if (Settings_stringIsSet(numberID))
	{
		Serial.println(Settings_getString(numberID));
	}
}

bool APP_SMS_Setup(void)
{
	s_linkitoneSMSInterface = SMS_GetInterface(SMS_INTERFACE_LINKITONE);

	/* There can be up to 4 generaly and 4 maintenance phone numbers set in settings */
	Serial.println("General SMS numbers:");
	printNumber(GENERAL_PHONE_NUMBER_1);
	printNumber(GENERAL_PHONE_NUMBER_2);
	printNumber(GENERAL_PHONE_NUMBER_3);
	printNumber(GENERAL_PHONE_NUMBER_4);
	Serial.println("Maintenance SMS numbers:");
	printNumber(MAINTENANCE_PHONE_NUMBER_1);
	printNumber(MAINTENANCE_PHONE_NUMBER_2);
	printNumber(MAINTENANCE_PHONE_NUMBER_3);
	printNumber(MAINTENANCE_PHONE_NUMBER_4);
}

void APP_SMS_SetDebug(bool on)
{
	s_debugSMS = on;
}

void APP_SMS_SendMessageToMaintenance(char * message)
{
	if (!message) { return; }

	sendMessage(MAINTENANCE_PHONE_NUMBER_1, message);
	sendMessage(MAINTENANCE_PHONE_NUMBER_2, message);
	sendMessage(MAINTENANCE_PHONE_NUMBER_3, message);
	sendMessage(MAINTENANCE_PHONE_NUMBER_4, message);
}
