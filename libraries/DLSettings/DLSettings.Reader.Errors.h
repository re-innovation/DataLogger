#ifndef _DL_SETTINGS_READER_ERRORS_H_
#define _DL_SETTINGS_READER_ERRORS_H_

/*
 * Defines and Typedefs
 */

enum settings_reader_result
{
	/* Shared errors (both global and channel settings) */
	ERR_READER_NONE,
    ERR_READER_NO_FILE,			// The specified settings file was not found
    ERR_READER_NO_INTERFACE,	// There was no storage interface pointer provided
    ERR_READER_NO_EQUALS,		// No equals sign in settings line

    /* Global setting errors */
    ERR_READER_NO_STRING,	// NULL pointer provided to parser
    ERR_READER_NO_NAME,		// Parsed setting name not found
    ERR_READER_INVALID_INT,	// String expected to be an int could not be parsed as such
    
    /* Channel setting errors */
    ERR_READER_NO_MANAGER,				// No datafield manager pointer was provided
    ERR_READER_NO_CHANNEL,				// Channel number could not be parsed from setting line
    ERR_READER_NO_SETTING,				// Setting name could not be parsed from setting line
    ERR_READER_UNKNOWN_TYPE,			// Channel type was parsed but not recognised
    ERR_READER_UNKNOWN_SETTING,			// Setting name was parsed but not recognised
    ERR_READER_INVALID_SETTING,			// Setting value was not valid
    ERR_READER_CHANNEL_TYPE_NOT_SET		// Channel type has not been set (channel type must be first setting for that channel)
};
typedef enum settings_reader_result SETTINGS_READER_RESULT;

// Strings are #-defined here so they may be used by both module and the test suite.

#define ERROR_STR_NO_FILE		"Settings file '%s' was not found."
#define ERROR_STR_NO_INTERFACE	"No storage interface provided for reading file."
#define ERROR_STR_NO_EQUALS     "Error on line %d: No equals sign found"

#define ERROR_STR_NO_STRING     "No string was supplied for setting line."
#define ERROR_STR_NO_NAME       "Error on line %d: Setting name '%s' was not recognised."
#define ERROR_STR_INVALID_INT   "Error on line %d: Setting '%s' for '%s' is not a whole number."
	
#define ERROR_STR_NO_MANAGER			"No datafield manager provided for reading file."
#define ERROR_STR_NO_CHANNEL			"Error on line %d: No channel found."
#define ERROR_STR_NO_SETTING			"Error on line %d: No setting name found."
#define ERROR_STR_UNKNOWN_TYPE			"Error on line %d: Unknown channel type '%s'."
#define ERROR_STR_UNKNOWN_SETTING		"Error on line %d: Unknown setting name in '%s'."
#define ERROR_STR_INVALID_SETTING		"Error on line %d: Invalid setting in '%s'."
#define ERROR_STR_CHANNEL_TYPE_NOT_SET	"Error on line %d: No type set for channel %d."

SETTINGS_READER_RESULT noError(void);
SETTINGS_READER_RESULT noFile(char const * pFilename);
SETTINGS_READER_RESULT noInterfaceError(void);
SETTINGS_READER_RESULT noEqualsError(int lineNo);

SETTINGS_READER_RESULT noStringError(void);
SETTINGS_READER_RESULT noNameError(char * pName, int lineNo);
SETTINGS_READER_RESULT invalidIntError(char * pName, char * pSetting, int lineNo);

SETTINGS_READER_RESULT noManagerError(void);
SETTINGS_READER_RESULT noChannelError(int lineNo);
SETTINGS_READER_RESULT noSettingError(int lineNo);
SETTINGS_READER_RESULT unknownTypeError(int lineNo, char * pType);
SETTINGS_READER_RESULT unknownSettingError(int lineNo, char * pSetting);
SETTINGS_READER_RESULT invalidSettingError(int lineNo, char * pSetting);
SETTINGS_READER_RESULT channelNotSetError(int lineNo, int channel);


SETTINGS_READER_RESULT Settings_getLastReaderResult(void);
char const * Settings_getLastReaderResultText(void);

#endif
