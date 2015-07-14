#ifndef _DLERROR_H_
#define _DLERROR_H_

// Fatal errors (from which application does not recover)
#define ERR_TYPE_NONE
#define ERR_TYPE_FATAL_CONFIG 1
#define ERR_TYPE_FATAL_CHANNEL 2
#define ERR_TYPE_FATAL_RUNTIME 3

// Error states in which the application can continue to run
// These are listed in ascending order of priority (batt > data upload > GPS)
enum error_enum
{
    ERR_NO_GPS,
    ERR_DATA_UPLOAD_FAILED,
    ERR_BATT_LEVEL_LOW,
	ERR_MAX
};
typedef enum error_enum ERROR_ENUM;

void Error_Tick(void);
void Error_Set_LED(uint8_t error_led);
void Error_Fatal(char const * const err, int nErrorType);
void Error_Running(ERROR_ENUM error, bool set);

#endif
