
/*
 * DLError.cpp
 *
 * James Fowkes
 *
 * 06 April 2015
 *
 * Error handling for datalogger applications
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

#include "TaskAction.h"

/*
 * Application Includes
 */

#include "DLError.h"

/*
 * Private Module Variables
 */

// Flags for each application error
static bool s_error_states[ERR_MAX];

// LED delay for each application error (defines 1/2 of flash period)
static uint16_t s_error_flash_timing[ERR_MAX] = {
	500,	// 1 flash per second
	250,	// 2 flashes per second
	100		// 5 flashes per second
};

static ERROR_ENUM s_current_error = ERR_MAX;

static uint8_t s_error_led_pin = 13;

/*
 * Private Module Functions
 */

static void errorLEDTaskFn(void)
{
	static bool ledon = false;

	if (s_current_error == ERR_MAX)
	{
        // No error,  turn LED off
		ledon = false;
		digitalWrite(s_error_led_pin, ledon);
	}
	else
	{
		digitalWrite(s_error_led_pin, ledon = !ledon);
	}
}
TaskAction errorLEDTask(errorLEDTaskFn, 0, INFINITE_TICKS);

/*
 * Public Module Functions
 */

 void Error_Tick(void)
 {
 	errorLEDTask.tick();
 }

/*
 * Error_Set_LEDs
 *
 * Set error LED pins
 */

void Error_Set_LEDs(uint8_t error_led)
{
	s_error_led_pin = error_led;	
}

/*
 * Error_Fatal
 *
 * Prints error to serial and then loops flashing LED
 */
void Error_Fatal(char const * const err, int nErrorType)
{
    Serial.println("Application error:");
    Serial.println(err);

    int i;
    #ifdef ARDUINO
    while (1)
    {
        for (i = 0; i < nErrorType; ++i)
        {
            digitalWrite(s_error_led_pin, HIGH);
            delay(200);
            digitalWrite(s_error_led_pin, LOW);
            delay(200);
        }
        delay(2000);
    }
    #else
    (void)i; (void)nErrorType;
    _exitMock();
    #endif    
}

/*
 * Error_Running
 *
 * Sets error LED timing
 */

void Error_Running(ERROR_ENUM error, bool set)
{
	s_error_states[error] = set;

	int i;

    s_current_error = ERR_MAX; // Assume no current error

    for (i = ERR_MAX-1; i >= (ERROR_ENUM)0; i--)
    {
    	if (s_error_states[i])
    	{
    		s_current_error = (ERROR_ENUM)i;
    		errorLEDTask.SetInterval(s_error_flash_timing[i]);
            break;
    	}
    }  
}
