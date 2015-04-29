/*
 * LinkItONE.OnboardADC.Example
 *
 * Onboard ADC example on the LinkItONE platform.
 *
 * See README.md for full details
 *
 * Basic summary:
 *
 * - The LinkIt ONE board has three 10-bit onboard ADCs
 * - Reads values every 5 seconds
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

/*
 * DataLogger Includes
 */

#include "DLUtility.h"
#include "DLSettings.h"
#include "DLSensor.LinkItONE.h"
#include "DLDataField.h"

static LinkItONEADC s_adcs[] = {
    LinkItONEADC(A0),
    LinkItONEADC(A1),
    LinkItONEADC(A2)
};

void setup()
{
    // setup Serial port
    Serial.begin(115200);

    delay(10000);

}

void loop()
{

    Serial.print("Raw ADC readings: ");
    Serial.print(s_adcs[0].read());    
    Serial.print(", ");
    Serial.print(s_adcs[1].read());    
    Serial.print(", ");
    Serial.print(s_adcs[2].read());    
    Serial.println("");
           
    delay(1000);
}


