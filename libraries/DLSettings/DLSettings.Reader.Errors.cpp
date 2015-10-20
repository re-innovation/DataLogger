/*
 * DLSettings.Reader.Errors.cpp
 *
 * Handles error strings and codes for settings readers
 *
 * Author: James Fowkes
 *
 * www.re-innovation.co.uk
 */

/*
 * Standard Library Includes
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * Datalogger Library Includes
 */

#include "DLSettings.Reader.Errors.h"

/*
 * Private Variables
 */

static char const * const s_errorStrings[] = {
	"", 
	ERROR_STR_NO_FILE,
	ERROR_STR_NO_INTERFACE,
	ERROR_STR_NO_EQUALS,

	ERROR_STR_NO_STRING,
	ERROR_STR_NO_NAME,
	ERROR_STR_INVALID_INT,
	
	ERROR_STR_NO_MANAGER,
	ERROR_STR_NO_CHANNEL,
	ERROR_STR_NO_SETTING,
	ERROR_STR_UNKNOWN_TYPE,
	ERROR_STR_UNKNOWN_SETTING,
	ERROR_STR_INVALID_SETTING,
	ERROR_STR_CHANNEL_TYPE_NOT_SET
};

static char s_errorBuffer[100] = "";

static SETTINGS_READER_RESULT s_lastResult = ERR_READER_NONE;

/*
 * Public Functions - Erros for both global settings and channel settings
 */

SETTINGS_READER_RESULT noError(void)
{
	s_lastResult = ERR_READER_NONE;
	s_errorBuffer[0] = '\0';
	return s_lastResult;
}

SETTINGS_READER_RESULT noFile(char const * pFilename)
{
	s_lastResult = ERR_READER_NO_FILE;
	sprintf(s_errorBuffer, s_errorStrings[ERR_READER_NO_FILE], pFilename);
	return s_lastResult;
}

SETTINGS_READER_RESULT noInterfaceError(void)
{
	s_lastResult = ERR_READER_NO_INTERFACE;
	sprintf(s_errorBuffer, s_errorStrings[ERR_READER_NO_INTERFACE]);
	return s_lastResult;
}

SETTINGS_READER_RESULT noEqualsError(int lineNo)
{
    s_lastResult = ERR_READER_NO_EQUALS;
    sprintf(s_errorBuffer, s_errorStrings[ERR_READER_NO_EQUALS], lineNo);
    return s_lastResult;
}

/*
 * Public Functions - Errors for global settings only
 */

SETTINGS_READER_RESULT noStringError(void)
{
	s_lastResult = ERR_READER_NO_STRING;
	sprintf(s_errorBuffer, s_errorStrings[ERR_READER_NO_STRING]);
	return s_lastResult;
}

SETTINGS_READER_RESULT noNameError(char * pName, int lineNo)
{
	s_lastResult = ERR_READER_NO_NAME;
	sprintf(s_errorBuffer, s_errorStrings[ERR_READER_NO_NAME], lineNo, pName);
	return s_lastResult;
}

SETTINGS_READER_RESULT invalidIntError(char * pName, char * pSetting, int lineNo)
{
	s_lastResult = ERR_READER_INVALID_INT;
	sprintf(s_errorBuffer, s_errorStrings[ERR_READER_INVALID_INT], lineNo, pSetting, pName);
	return s_lastResult;
}

/*
 * Public Functions - Errors for data channel settings only
 */

SETTINGS_READER_RESULT noManagerError(void)
{
    s_lastResult = ERR_READER_NO_MANAGER;
    sprintf(s_errorBuffer, s_errorStrings[ERR_READER_NO_MANAGER]);
    return s_lastResult;
}

SETTINGS_READER_RESULT noChannelError(int lineNo)
{
    s_lastResult = ERR_READER_NO_CHANNEL;
    sprintf(s_errorBuffer, s_errorStrings[ERR_READER_NO_CHANNEL], lineNo);
    return s_lastResult;
}

SETTINGS_READER_RESULT noSettingError(int lineNo)
{
    s_lastResult = ERR_READER_NO_SETTING;
    sprintf(s_errorBuffer, s_errorStrings[ERR_READER_NO_SETTING], lineNo);
    return s_lastResult;  
}

SETTINGS_READER_RESULT unknownTypeError(int lineNo, char * pType)
{
    s_lastResult = ERR_READER_UNKNOWN_TYPE;
    sprintf(s_errorBuffer, s_errorStrings[ERR_READER_UNKNOWN_TYPE], lineNo, pType);
    return s_lastResult;  
}

SETTINGS_READER_RESULT unknownSettingError(int lineNo, char * pSetting)
{
    s_lastResult = ERR_READER_UNKNOWN_SETTING;
    sprintf(s_errorBuffer, s_errorStrings[ERR_READER_UNKNOWN_SETTING], lineNo, pSetting);
    return s_lastResult;
}

SETTINGS_READER_RESULT invalidSettingError(int lineNo, char * pSetting)
{
    s_lastResult = ERR_READER_INVALID_SETTING;
    sprintf(s_errorBuffer, s_errorStrings[ERR_READER_INVALID_SETTING], lineNo, pSetting);
    return s_lastResult;
}

SETTINGS_READER_RESULT channelNotSetError(int lineNo, int channel)
{
    s_lastResult = ERR_READER_CHANNEL_TYPE_NOT_SET;
    sprintf(s_errorBuffer, s_errorStrings[ERR_READER_CHANNEL_TYPE_NOT_SET], lineNo, channel+1);
    return s_lastResult;  
}

/*
 * Public Functions - Error module result and strings
 */

SETTINGS_READER_RESULT Settings_getLastReaderResult(void)
{
	return s_lastResult;
}

char const * Settings_getLastReaderResultText(void)
{
	return s_errorBuffer;
}
