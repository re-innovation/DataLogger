#ifndef _DL_SETTINGS_GLOBAL_H_
#define _DL_SETTINGS_GLOBAL_H_

/*
 * Application settings are defined using preprocessor magic.
 * This is so the enumeration numbers and the strings can be kept in sync.
 * For example, GPRS_APN and the string "GPRS_APN" are both generated from the same
 * definition. 
 */

/* Define each string setting for the application */

#define FOREACH_STRINGSET(STRING) \
    STRING(UNIT_IDENTIFIER) \
    STRING(FAKE_ADC_READS) \
    STRING(DEBUG_MODULES) \
    STRING(DEBUG_FIELDS) \
    STRING(GPRS_APN) \
    STRING(GPRS_USERNAME) \
    STRING(GPRS_PASSWORD) \
    STRING(THINGSPEAK_URL) \
    STRING(THINGSPEAK_API_KEY) \
    STRING(GENERAL_PHONE_NUMBER_1) \
    STRING(GENERAL_PHONE_NUMBER_2) \
    STRING(GENERAL_PHONE_NUMBER_3) \
    STRING(GENERAL_PHONE_NUMBER_4) \
    STRING(MAINTENANCE_PHONE_NUMBER_1) \
    STRING(MAINTENANCE_PHONE_NUMBER_2) \
    STRING(MAINTENANCE_PHONE_NUMBER_3) \
    STRING(MAINTENANCE_PHONE_NUMBER_4)

/* Define each int setting for the application */

#define FOREACH_INTSET(INT) \
    INT(DATA_UPLOAD_INTERVAL_SECS) \
    INT(UPLOAD_AVERAGING_INTERVAL_SECS) \
    INT(STORAGE_AVERAGING_INTERVAL_SECS) \
    INT(DATA_STORAGE_INTERVAL_SECS) \
    INT(SERIAL_DATA_INTERVAL_SECS) \
    INT(BATTERY_WARN_INTERVAL_MINUTES) \
    INT(BATTERY_WARN_LEVEL) \
    INT(ENABLE_DATA_DEBUG)
    
#define GENERATE_ENUM(ENUM) ENUM, // This turns each setting into an enum entry
#define GENERATE_STRING(STRING) #STRING, // This turns each setting into a string in an array

/* Generate string enumeration */
enum stringsetting
{
    FOREACH_STRINGSET(GENERATE_ENUM)
    STRING_SETTINGS_COUNT
};
typedef enum stringsetting STRINGSETTING;

/* Generate int enumeration */
enum intsetting
{
    FOREACH_INTSET(GENERATE_ENUM)
    INT_SETTINGS_COUNT
};
typedef enum intsetting INTSETTING;

typedef void (*PRINTFN)(char const * const);

// Each setting is either a string or an integer.
// If an incorrect request is made (e.g. expecting a char * from an integer),
// or if that setting has not been set, a null value is returned instead.
// 0 is returned for integers, NULL for strings

void Settings_InitGlobal(void);
    
char const * Settings_getIntName(INTSETTING setting);
int32_t Settings_getInt(INTSETTING setting);
void Settings_setInt(INTSETTING setting, int32_t set);
bool Settings_intIsSet(INTSETTING setting);
void Settings_resetInt(INTSETTING setting);

char const * Settings_getStringName(STRINGSETTING setting);
char * Settings_getString(STRINGSETTING setting);
void Settings_setString(STRINGSETTING setting, char const * const pSet);
bool Settings_stringIsSet(STRINGSETTING setting);
void Settings_resetString(STRINGSETTING setting);

uint8_t Settings_getIntCount(void);
uint8_t Settings_getStringCount(void);
uint8_t Settings_getCount(void);

void Settings_echoAllSet(PRINTFN printfn);

#endif