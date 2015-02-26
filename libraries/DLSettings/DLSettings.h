#ifndef _SETTINGS_H_
#define _SETTINGS_H_

/* This enumeration defines every possible string setting for the system,
 * divided into sensible blocks
 */
enum stringsetting
{
    /*** Network settings ***/
    /** GPRS **/
    GPRS_APN,
    GPRS_USERNAME,
    GPRS_PASSWORD,
    
    /*** Internet Services ***/
    /** Thingspeak **/
    THINGSPEAK_URL,
    THINGSPEAK_API_KEY,
    
    /*** MUST be the last entry in the enumeration! ***/
    STRING_SETTINGS_COUNT
};
typedef enum stringsetting STRINGSETTING;

enum intsetting
{
    CSV_RECORD_INTERVAL,

    /*** MUST be the last entry in the enumeration! ***/
    INT_SETTINGS_COUNT
};
typedef enum intsetting INTSETTING;

// Each setting is either a string or an integer.
// If an incorrect request is made (e.g. expecting a char * from an integer),
// or if that setting has not been set, a null value is returned instead.
// 0 is returned for integers, NULL for strings

int Settings_getInt(INTSETTING setting);
void Settings_setInt(INTSETTING setting, int set);

char * Settings_getString(STRINGSETTING setting);
void Settings_setString(STRINGSETTING setting, char const * const pSet);


#endif