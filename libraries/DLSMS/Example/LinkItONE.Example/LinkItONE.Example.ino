/*
 * LinkIT ONE
 *
 * SMS example on the LinkItONE platform.
 *
 * See README.md for full details
 *
 * Basic summary:
 *
 * - Sends example SMS message
 */

/*
 * Standard Library Includes
 */
#include <stdint.h>

/*
 * Arduino Library Includes
 */

#include <Wire.h>

/*
 * LinkIt One Includes
 */

#include <LGSM.h>

/*
 * DataLogger Includes
 */

#include "DLSMS.h"

// Pointers to fuctionality objects
static SMSInterface * s_linkitoneSMSInterface;

void setup()
{
    // setup Serial port
    Serial.begin(115200);

    delay(10000);
    
    s_linkitoneSMSInterface = SMS_GetInterface(SMS_INTERFACE_LINKITONE);

    if (s_linkitoneSMSInterface->send("07411048014", "Hello, world!"))
    {
    	Serial.println("Message sent!");
    }
    else
   	{
   		Serial.println("Send failed!");
   	};
}

void loop()
{
    
}
