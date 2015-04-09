#ifndef _DL_SETTINGS_H_
#define _DL_SETTINGS_H_

/*
 * Application settings are defined using preprocessor magic.
 * This is so the enumeration numbers and the strings can be kept in sync.
 * For example, GPRS_APN and the string "GPRS_APN" are both generated from the same
 * definition. 
 */

/* Define each string setting for the application */

#define FOREACH_STRINGSET(STRING) \
    STRING(GPRS_APN) \
    STRING(GPRS_USERNAME) \
    STRING(GPRS_PASSWORD) \
    STRING(THINGSPEAK_URL) \
    STRING(THINGSPEAK_API_KEY)

/* Define each int setting for the application */

#define FOREACH_INTSET(INT) \
    INT(CSV_RECORD_INTERVAL) \
    INT(SERIAL_DATA_INTERVAL) \

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

// Each setting is either a string or an integer.
// If an incorrect request is made (e.g. expecting a char * from an integer),
// or if that setting has not been set, a null value is returned instead.
// 0 is returned for integers, NULL for strings

char const * Settings_getIntName(INTSETTING setting);
int Settings_getInt(INTSETTING setting);
void Settings_setInt(INTSETTING setting, int set);

char const * Settings_getStringName(STRINGSETTING setting);
char * Settings_getString(STRINGSETTING setting);
void Settings_setString(STRINGSETTING setting, char const * const pSet);


#endif