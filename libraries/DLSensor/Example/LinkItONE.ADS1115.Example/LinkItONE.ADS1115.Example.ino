/*
 * LinkIT ONE
 *
 * ADS1115 example on the LinkItONE platform.
 *
 * See README.md for full details
 *
 * Basic summary:
 *
 * - Interfaces with 3 ADS1115 sensors on the i2c bus.
 * - Reads values every 5 seconds and sends to serial port
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
#include "DLSensor.ADS1x1x.h"
#include "DLDataField.h"

static ADS1115 s_adcs[] = {
    ADS1115(0x48),
    ADS1115(0x49),
    ADS1115(0x4A)
};

void setup()
{
    // setup Serial port
    Serial.begin(115200);
    
    uint8_t i = 0;
    for (i = 0; i < 3; i++)
    {
        s_adcs[i].begin();
    }

    delay(10000);

}

void loop()
{

    uint8_t i = 0;
    uint8_t ch = 0;
    
    for (i = 0; i < 3; i++)
    {
        int16_t adc0, adc1, adc2, adc3;

        adc0 = s_adcs[i].readADC_SingleEnded(0);
        adc1 = s_adcs[i].readADC_SingleEnded(1);
        adc2 = s_adcs[i].readADC_SingleEnded(2);
        adc3 = s_adcs[i].readADC_SingleEnded(3);
        Serial.print("ADS1115-");
        Serial.print(i);
        Serial.println(":");
        Serial.print("AIN0: "); Serial.println(adc0);
        Serial.print("AIN1: "); Serial.println(adc1);
        Serial.print("AIN2: "); Serial.println(adc2);
        Serial.print("AIN3: "); Serial.println(adc3);
        Serial.println(" ");
    }
       
    delay(1000);
}


