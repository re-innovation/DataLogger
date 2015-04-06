/*
 * LinkItONE.Thermistor.Example
 *
 * Onboard ADC example on the LinkItONE platform.
 *
 * See README.md for full details
 *
 * Basic summary:
 *
 * - The LinkIt ONE board has three 10-bit onboard ADCs
 * - Reads and echoes thermistor values every 5 seconds
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
#include "DLSensor.Thermistor.h"

static LinkItONEADC s_adcs[] = {
    LinkItONEADC(A0),
    LinkItONEADC(A1),
    LinkItONEADC(A2)
};

static Thermistor s_thermistors[] = {
    Thermistor(3977, 10000),
    Thermistor(3977, 10000),
    Thermistor(3977, 10000)
};

void setup()
{
    // setup Serial port
    Serial.begin(115200);

    delay(10000);

}

void loop()
{

    Serial.print("Thermistor readings (raw ADC): ");
    Serial.print(s_thermistors[0].TemperatureFromADCReading(10000.0, s_adcs[0].read(), 675));
    Serial.print("(");
    Serial.print(s_adcs[0].read());
    Serial.print(")");
    Serial.print(", ");
    
    Serial.print(s_thermistors[1].TemperatureFromADCReading(10000.0, s_adcs[1].read(), 675));
    Serial.print("(");
    Serial.print(s_adcs[1].read());
    Serial.print(")");
    Serial.print(", ");
    
    Serial.print(s_thermistors[2].TemperatureFromADCReading(10000.0, s_adcs[2].read(), 675));
    Serial.print("(");
    Serial.print(s_adcs[2].read());
    Serial.println(")");
           
    delay(1000);
}


