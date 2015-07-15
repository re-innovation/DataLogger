/*
 * LinkItONE.ErrorsTest
 *
 * Test/Example Application for applicaiton error handling on LinkItONE hardware
 *
 * See README.md for full details
 *
 * Basic summary:
 *
 * - Simulates running application errors
 *
 */

/*
 * Standard Library Includes
 */

#include <stdint.h>


/*
 * DataLogger Includes
 */

#include "DLError.h"
#include "TaskAction.h"

/*
 * Application Includes
 */

#include "app.h"
#include "app.error.h"

// Pointers to fuctionality objects
static void delayStart(uint8_t seconds)
{
    Serial.println("Starting application in... ");

    while (seconds--)
    {
        Serial.print(seconds);
        Serial.print("...");
        delay(1000);
    }
    Serial.println();
}

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

TaskAction errorSimulationTask(errorSimulationTaskFn, 6000, INFINITE_TICKS);
static void errorSimulationTaskFn(void)
{
    static bool turnOnErrors = true;
    static int nextError = ERR_RUNNING_NO_GPS;

    Serial.print("Turning error ");  
    Serial.print(nextError);
    Serial.println(turnOnErrors ? " on" : " off");

    Error_Running((RUNNING_ERROR_ENUM)(nextError++), turnOnErrors);
      
    if(nextError == ERR_RUNNING_MAX)
    {
        nextError = ERR_RUNNING_NO_GPS;
        turnOnErrors = !turnOnErrors;
    }
}

void setup()
{   
    // setup Serial port
    Serial.begin(115200);

    delayStart(10);

    APP_Error_Setup();

    pinMode(HEARTBEAT_LED_PIN, OUTPUT);
    pinMode(ERROR_LED_PIN, OUTPUT);
}

void loop()
{
    errorSimulationTask.tick();
    APP_Error_Tick();
}

