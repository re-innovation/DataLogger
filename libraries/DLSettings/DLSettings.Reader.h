#ifndef _DL_SETTINGS_READER_H_
#define _DL_SETTINGS_READER_H_

/*
 * Defines and Typedefs
 */

enum settings_reader_result
{
    ERR_NONE,
    ERR_NO_STRING,
    ERR_NO_EQUALS,
    ERR_NO_NAME,
    ERR_INVALID_INT
};
typedef enum settings_reader_result SETTINGS_READER_RESULT;

// Strings are #-defined here so they may be used by both module and the test suite.
#define ERROR_STR_NO_STRING     "No string was supplied for setting line."
#define ERROR_STR_NO_EQUALS     "No equals sign found in setting line."
#define ERROR_STR_NO_NAME       "Setting name '%s' was not found."
#define ERROR_STR_INVALID_INT   "Setting '%s' for '%s' is not a whole number."

/*
 * Public Functions
 */

SETTINGS_READER_RESULT Settings_getLastReaderResult(void);
SETTINGS_READER_RESULT Settings_readFromString(char const * const string);
char const * Settings_getLastReaderResultText(void);

#ifdef TEST
void Settings_resetReader(void);
#endif

#endif
