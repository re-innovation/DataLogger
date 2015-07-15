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

static SMSInterface * s_sms_interface;

void SMS_Setup(SMS_INTERFACE interface_type)
{
    s_sms_interface = SMS_GetInterface(interface_type);
}

void SMS_SendGeneralMessage(char * message)
{
    if (!message) { return; }

    /* There can be up to 4 phone numbers set in settings */
    if (Settings_stringIsSet(GENERAL_PHONE_NUMBER_1))
    {
        s_sms_interface->send(Settings_getString(GENERAL_PHONE_NUMBER_1), message);
    }
    
    if (Settings_stringIsSet(GENERAL_PHONE_NUMBER_2))
    {
        s_sms_interface->send(Settings_getString(GENERAL_PHONE_NUMBER_2), message);
    }
    
    if (Settings_stringIsSet(GENERAL_PHONE_NUMBER_3))
    {
        s_sms_interface->send(Settings_getString(GENERAL_PHONE_NUMBER_3), message);
    }

    if (Settings_stringIsSet(GENERAL_PHONE_NUMBER_4))
    {
        s_sms_interface->send(Settings_getString(GENERAL_PHONE_NUMBER_4), message);
    }   
}

void SMS_SendMaintenanceMessage(char * message)
{
    if (!message) { return; }

    /* There can be up to 4 phone numbers set in settings */
    if (Settings_stringIsSet(MAINTENANCE_PHONE_NUMBER_1))
    {
        s_sms_interface->send(Settings_getString(MAINTENANCE_PHONE_NUMBER_1), message);
    }
    
    if (Settings_stringIsSet(MAINTENANCE_PHONE_NUMBER_2))
    {
        s_sms_interface->send(Settings_getString(MAINTENANCE_PHONE_NUMBER_2), message);
    }
    
    if (Settings_stringIsSet(MAINTENANCE_PHONE_NUMBER_3))
    {
        s_sms_interface->send(Settings_getString(MAINTENANCE_PHONE_NUMBER_3), message);
    }

    if (Settings_stringIsSet(MAINTENANCE_PHONE_NUMBER_4))
    {
        s_sms_interface->send(Settings_getString(MAINTENANCE_PHONE_NUMBER_4), message);
    }   
}