#ifndef _APP_ERRORS_H_
#define _APP_ERRORS_H_

// Fatal errors (from which application does not recover)
#define ERR_TYPE_NONE
#define ERR_TYPE_FATAL_CONFIG 1
#define ERR_TYPE_FATAL_CHANNEL 2
#define ERR_TYPE_FATAL_RUNTIME 3

// Error state in which the application can continue to run
enum app_error_enum
{
	ERR_NONE,
    ERR_NO_GPS,
    ERR_DATA_UPLOAD_FAILED,
	ERR_BATT_LEVEL_LOW
};
typedef enum app_error_enum APP_ERROR_ENUM;

#endif