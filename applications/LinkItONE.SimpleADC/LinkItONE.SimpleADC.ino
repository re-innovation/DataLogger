/*
 * LinkItONE.SimpleADC
 *
 * ADC Debugging for on LinkItONE hardware
 *
 * See README.md for full details
 *
 * Basic summary:
 *
 * - Reads data from ADS1115 ADCs (addresses 0x48, 0x49, 0x4A) once per second
 * - and echoes to serial
 *
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
 * DataLogger Includes
 */

#include "DLSensor.ADS1x1x.h"
#include "DLSensor.LinkItONE.h"

#include "TaskAction.h"

#define HEARTBEAT_LED_PIN (5)
#define ERROR_LED_PIN (4)

// Pointers to fuctionality objects
static ADS1115 s_ADCs[] = {
    ADS1115(0x48),
    ADS1115(0x49),
    ADS1115(0x4A)
};

static LinkItONEADC s_internalADCs[] = {
    LinkItONEADC(A0),
    LinkItONEADC(A1),
    LinkItONEADC(A2)
};

/*
 * Tasks
 */
static void heartbeatTaskFn(void);
TaskAction heartbeatTask(heartbeatTaskFn, 1000, INFINITE_TICKS);
static void heartbeatTaskFn(void)
{
    static bool ledState = false;
    digitalWrite(HEARTBEAT_LED_PIN, ledState ? HIGH : LOW);
    // 1s on, 4s off by setting task interval
    heartbeatTask.SetInterval(ledState ? 1000 : 4000);
    ledState = !ledState;
}

static void readFromADCsTaskFn(void)
{
    uint8_t adc = 0;
    uint8_t ch = 0;
    uint8_t field = 0;
    int32_t reading;

    // Read the ADC1x1x ICs for fields 1 - 12
    for (adc = 0; adc < 3; adc++)
    {
        for (ch = 0; ch < 4; ch++)
        {
            field = (adc*4)+ch;
            reading = s_ADCs[adc].readADC_SingleEnded(ch);
            Serial.print(reading);
            Serial.print(", ");
        }
    }

    // Read the internal ADCs for fields 13-15
    for (adc = 0; adc < 3; adc++)
    {
        field = adc + 12;
        reading = s_internalADCs[adc].read();
        Serial.print(reading);
        Serial.print(", ");
    }
    Serial.println("");
}
TaskAction readFromADCsTask(readFromADCsTaskFn, 1000, INFINITE_TICKS, "ADC Read Task");

void setupADCs(void)
{
    uint8_t i = 0;
    for (i = 0; i < 3; i++)
    {
        s_ADCs[i].begin();
        s_ADCs[i].setGain(GAIN_ONE);
    }
}

void setup()
{   
    // setup Serial port
    Serial.begin(115200);

    setupADCs();    
}

void loop()
{
    readFromADCsTask.tick();
    heartbeatTask.tick();
}

