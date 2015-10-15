/*
 * DLSMS.messaging.cpp
 * 
 * Interface for sending general/maintenance SMS messages
 *
 * Author: James Fowkes
 *
 * www.re-innovation.co.uk
 */


/*
 * Arduino Library Includes
 */

#include <Arduino.h>

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
#include "DLSMS.messaging.h"

#include "DLUtility.Strings.h"

static SMSInterface * s_sms_interface;

static char s_localMessageBuffer[140];
static FixedLengthAccumulator s_accumulator(NULL, 0);

/*
 * Private Functions
 */

static void put_id_and_message_to_buffer(char * message)
{
    s_accumulator.reset();
    s_accumulator.attach(s_localMessageBuffer, 140);

    if (Settings_stringIsSet(UNIT_IDENTIFIER))
    {
        s_accumulator.writeString("From ");
        s_accumulator.writeString(Settings_getString(UNIT_IDENTIFIER));
        s_accumulator.writeString(": ");
    }

    s_accumulator.writeString(message);
}

/*
 * Public Functions
 */

void SMS_Setup(SMS_INTERFACE interface_type)
{
    s_sms_interface = SMS_GetInterface(interface_type);
}

void SMS_SendGeneralMessage(char * message)
{
    if (!message) { return; }

    put_id_and_message_to_buffer(message);

    /* There can be up to 4 phone numbers set in settings */
    if (Settings_stringIsSet(GENERAL_PHONE_NUMBER_1))
    {
        s_sms_interface->send(Settings_getString(GENERAL_PHONE_NUMBER_1), s_localMessageBuffer);
    }
    
    if (Settings_stringIsSet(GENERAL_PHONE_NUMBER_2))
    {
        s_sms_interface->send(Settings_getString(GENERAL_PHONE_NUMBER_2), s_localMessageBuffer);
    }
    
    if (Settings_stringIsSet(GENERAL_PHONE_NUMBER_3))
    {
        s_sms_interface->send(Settings_getString(GENERAL_PHONE_NUMBER_3), s_localMessageBuffer);
    }

    if (Settings_stringIsSet(GENERAL_PHONE_NUMBER_4))
    {
        s_sms_interface->send(Settings_getString(GENERAL_PHONE_NUMBER_4), s_localMessageBuffer);
    }
}

void SMS_SendMaintenanceMessage(char * message)
{
    if (!message) { return; }

    put_id_and_message_to_buffer(message);

    Serial.print("Sending message ");
    Serial.println(s_localMessageBuffer);

    /* There can be up to 4 phone numbers set in settings */
    if (Settings_stringIsSet(MAINTENANCE_PHONE_NUMBER_1))
    {
        s_sms_interface->send(Settings_getString(MAINTENANCE_PHONE_NUMBER_1), s_localMessageBuffer);
    }
    
    if (Settings_stringIsSet(MAINTENANCE_PHONE_NUMBER_2))
    {
        s_sms_interface->send(Settings_getString(MAINTENANCE_PHONE_NUMBER_2), s_localMessageBuffer);
    }
    
    if (Settings_stringIsSet(MAINTENANCE_PHONE_NUMBER_3))
    {
        s_sms_interface->send(Settings_getString(MAINTENANCE_PHONE_NUMBER_3), s_localMessageBuffer);
    }

    if (Settings_stringIsSet(MAINTENANCE_PHONE_NUMBER_4))
    {
        s_sms_interface->send(Settings_getString(MAINTENANCE_PHONE_NUMBER_4), s_localMessageBuffer);
    }   
}