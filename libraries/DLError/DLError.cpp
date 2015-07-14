
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

#include <Arduino.h>
 
/*
 * Standard Library Includes
 */

#include <stdint.h>

/*
 * DataLogger Includes
 */

#include "DLError.h"

/*
 * Private Module Variables
 */

// Flags for each application error
static bool s_error_states[ERR_RUNNING_MAX];

static FATAL_ERROR_ENUM s_current_fatal_error = ERR_FATAL_NONE;
static RUNNING_ERROR_ENUM s_current_running_error = ERR_RUNNING_NONE;

static APP_FATAL_ERROR_HANDLER s_fatal_handler;
static APP_RUNNING_ERROR_HANDLER s_running_handler;

/*
 * Private Module Functions
 */

/*
 * Error_Setup
 *
 * Set error Lcallback functions
 */

void Error_Setup(APP_FATAL_ERROR_HANDLER fnFatalHandler, APP_RUNNING_ERROR_HANDLER fnRunningHandler)
{
	s_fatal_handler = fnFatalHandler;
    s_running_handler = fnRunningHandler;
    s_current_fatal_error = ERR_FATAL_NONE;
    s_current_running_error = ERR_RUNNING_NONE;
}

/*
 * Error_Fatal
 *
 * Calls back to application fatal error handler
 */
void Error_Fatal(char const * const err, FATAL_ERROR_ENUM errorType)
{
    s_current_fatal_error = errorType;
    if (s_fatal_handler)
    {
        s_fatal_handler(err, errorType);
    } 
}

/*
 * Error_Running
 *
 * Sets error LED timing
 */

void Error_Running(RUNNING_ERROR_ENUM error, bool set)
{
	s_error_states[error] = set;

	int i;

    s_current_running_error = ERR_RUNNING_NONE; // Assume no current error

    for (i = ERR_RUNNING_NONE+1; i < ERR_RUNNING_MAX; ++i)
    {
    	if (s_error_states[i])
    	{
    		s_current_running_error = (RUNNING_ERROR_ENUM)i;
    	}
    }  

    if (s_running_handler)
    {
        s_running_handler(s_current_running_error);
    }
}

/*
 * Error_Get_Running_Error
 *
 * Returns current running error
 */
RUNNING_ERROR_ENUM Error_Get_Running_Error(void)
{
    return s_current_running_error;
}

/*
 * Error_Get_Fatal_Error
 *
 * Returns current fatal error
 */

FATAL_ERROR_ENUM Error_Get_Fatal_Error(void)
{
    return s_current_fatal_error;
}