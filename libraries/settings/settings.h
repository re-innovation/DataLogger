#ifndef _SETTINGS_H_
#define _SETTINGS_H_

/* This enumeration defines every possible string setting for the system,
 * divided into sensible blocks
 */
enum stringsetting
{
    /*** Network settings ***/
    /** GPRS **/
    GRPS_APN,
    GPRS_USERNAME,
    GPRS_PASSWORD,
    
    /*** Internet Services ***/
    /** Thingspeak **/
    THINGSPEAK_API_KEY,
    
    /*** MUST be the last entry in the enumeration! ***/
    STRING_SETTINGS_COUNT
};
typedef enum setting STRINGSETTING;

enum intsetting
{
    /*** MUST be the last entry in the enumeration! ***/
    INT_SETTINGS_COUNT
};
typedef enum intsetting INTSETTING;

// Each setting is either a string or an integer.
// If an incorrect request is made (e.g. expecting a char * from an integer),
// or if that setting has not been set, a null value is returned instead.
// 0 is returned for integers, NULL for strings

char * GetString(STRINGSETTING setting);
int GetInt(INTSETTING setting);

void SetString(STRINGSETTING setting, char * pSet);
void GetInt(INTSETTING setting, int set);

#endif