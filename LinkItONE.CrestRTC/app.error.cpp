/*
 * app.error.cpp
 *
 * James Fowkes
 *
 * 06 April 2015
 *
 * Defines error handling for the Crest application
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

#include "DLError.h"
#include "TaskAction.h"

/*
 * Application Includes
 */

#include "app.h"

// LED delay for each application error (defines 1/2 of flash period)
static uint16_t s_error_flash_timing[ERR_RUNNING_MAX] = {
	500,	// No error, just keep task ticking over
	500,	// 1 flash per second
	250,	// 2 flashes per second
	100		// 5 flashes per second
};

static void errorTaskFn(void)
{
	static bool ledon = false;

	if (Error_Get_Running_Error() == ERR_RUNNING_NONE)
	{
        // No error,  turn LED off
		ledon = false;
		digitalWrite(ERROR_LED_PIN, ledon);
	}
	else
	{
		digitalWrite(ERROR_LED_PIN, ledon = !ledon);
	}
}
TaskAction errorTask(errorTaskFn, 0, INFINITE_TICKS);

static void fatal_error_handler(char const * const err, FATAL_ERROR_ENUM errorType)
{
    Serial.println("Application error:");
    Serial.println(err);

    int i;
    #ifdef ARDUINO
    while (1)
    {
        for (i = 0; i < errorType; ++i) // Use error type as # of times to flash LED
        {
            digitalWrite(ERROR_LED_PIN, HIGH);
            delay(200);
            digitalWrite(ERROR_LED_PIN, LOW);
            delay(200);
        }
        delay(2000);
    }
    #else
    (void)i; (void)errorType;
    _exitMock();
    #endif   
}

static void running_error_handler(RUNNING_ERROR_ENUM errorType)
{
	bool is_error = errorType != ERR_RUNNING_NONE;
	if (is_error)
	{
		errorTask.SetInterval(s_error_flash_timing[errorType]);
	}
}

void APP_Error_Setup(void)
{
	// Register error callbacks with library
	Error_Setup(fatal_error_handler, running_error_handler);
}

void APP_Error_Tick(void)
{
	errorTask.tick();
}
