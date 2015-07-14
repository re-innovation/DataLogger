#ifndef _DLERROR_H_
#define _DLERROR_H_

// Fatal errors (from which application does not recover)
enum fatal_error_enum
{
	ERR_FATAL_NONE,
	ERR_FATAL_CONFIG,
	ERR_FATAL_CHANNEL,
	ERR_FATAL_RUNTIME
};
typedef enum fatal_error_enum FATAL_ERROR_ENUM;

// Error states in which the application can continue to run
// These are listed in ascending order of priority (batt > data upload > GPS)
enum running_error_enum
{
	ERR_RUNNING_NONE,
    ERR_RUNNING_NO_GPS,
    ERR_RUNNING_DATA_UPLOAD_FAILED,
    ERR_RUNNING_BATT_LEVEL_LOW,
    ERR_RUNNING_MAX
};
typedef enum running_error_enum RUNNING_ERROR_ENUM;


/* 
 * The application must provide callback pointers to error handlers
 * that output the error to the user (via LEDs, serial etc.)
 */

typedef void (*APP_FATAL_ERROR_HANDLER)(char const * const err, FATAL_ERROR_ENUM errorType);
typedef void (*APP_RUNNING_ERROR_HANDLER)(RUNNING_ERROR_ENUM errorType);

void Error_Setup(APP_FATAL_ERROR_HANDLER fnFatalHandler, APP_RUNNING_ERROR_HANDLER fnRunningHandler);
void Error_Fatal(char const * const err, FATAL_ERROR_ENUM errorType);
void Error_Running(RUNNING_ERROR_ENUM error, bool set);

FATAL_ERROR_ENUM Error_Get_Fatal_Error(void);
RUNNING_ERROR_ENUM Error_Get_Running_Error(void);

#endif
