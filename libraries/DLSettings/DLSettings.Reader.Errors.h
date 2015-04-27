#ifndef _DL_SETTINGS_READER_ERRORS_H_
#define _DL_SETTINGS_READER_ERRORS_H_

/*
 * Defines and Typedefs
 */

enum settings_reader_result
{
    ERR_READER_NONE,
    ERR_READER_NO_STRING,
    ERR_READER_NO_EQUALS,
    ERR_READER_NO_NAME,
    ERR_READER_INVALID_INT,
    ERR_READER_NO_FILE,
    ERR_READER_NO_INTERFACE,
    ERR_READER_NO_MANAGER
};
typedef enum settings_reader_result SETTINGS_READER_RESULT;

// Strings are #-defined here so they may be used by both module and the test suite.
#define ERROR_STR_NO_STRING     "No string was supplied for setting line."
#define ERROR_STR_NO_EQUALS     "No equals sign found in setting line."
#define ERROR_STR_NO_NAME       "Setting name '%s' was not found."
#define ERROR_STR_INVALID_INT   "Setting '%s' for '%s' is not a whole number."
#define ERROR_STR_NO_FILE		"Settings file %s was not found."
#define ERROR_STR_NO_INTERFACE	"No storage interface provided for reading file."
#define ERROR_STR_NO_MANAGER	"No datafield manager provided for reading file."

SETTINGS_READER_RESULT noManagerError(void);
SETTINGS_READER_RESULT noStringError(void);
SETTINGS_READER_RESULT noEqualsError(void);
SETTINGS_READER_RESULT noNameError(char * pName);
SETTINGS_READER_RESULT invalidIntError(char * pName, char * pSetting);
SETTINGS_READER_RESULT noFile(char const * pFilename);
SETTINGS_READER_RESULT noInterfaceError(void);
SETTINGS_READER_RESULT noError(void);

SETTINGS_READER_RESULT Settings_getLastReaderResult(void);
char const * Settings_getLastReaderResultText(void);

#endif
