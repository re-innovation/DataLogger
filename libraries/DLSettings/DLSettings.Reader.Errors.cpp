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
	ERROR_STR_NO_STRING,
	ERROR_STR_NO_EQUALS,
	ERROR_STR_NO_NAME,
	ERROR_STR_INVALID_INT,
	ERROR_STR_NO_FILE,
	ERROR_STR_NO_INTERFACE,
	ERROR_STR_NO_MANAGER
};

static char s_errorBuffer[100] = "";
static SETTINGS_READER_RESULT s_lastResult = ERR_READER_NONE;

/*
 * Public Functions
 */

SETTINGS_READER_RESULT noManagerError(void)
{
    s_lastResult = ERR_READER_NO_MANAGER;
    sprintf(s_errorBuffer, s_errorStrings[ERR_READER_NO_MANAGER]);
    return s_lastResult;
}

SETTINGS_READER_RESULT noStringError(void)
{
	s_lastResult = ERR_READER_NO_STRING;
	sprintf(s_errorBuffer, s_errorStrings[ERR_READER_NO_STRING]);
	return s_lastResult;
}

SETTINGS_READER_RESULT noEqualsError(void)
{
	s_lastResult = ERR_READER_NO_EQUALS;
	sprintf(s_errorBuffer, s_errorStrings[ERR_READER_NO_EQUALS]);
	return s_lastResult;
}

SETTINGS_READER_RESULT noNameError(char * pName)
{
	s_lastResult = ERR_READER_NO_NAME;
	sprintf(s_errorBuffer, s_errorStrings[ERR_READER_NO_NAME], pName);
	return s_lastResult;
}

SETTINGS_READER_RESULT invalidIntError(char * pName, char * pSetting)
{
	s_lastResult = ERR_READER_INVALID_INT;
	sprintf(s_errorBuffer, s_errorStrings[ERR_READER_INVALID_INT], pSetting, pName);
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

SETTINGS_READER_RESULT noError(void)
{
	s_lastResult = ERR_READER_NONE;
	s_errorBuffer[0] = '\0';
	return s_lastResult;
}

SETTINGS_READER_RESULT Settings_getLastReaderResult(void)
{
	return s_lastResult;
}

char const * Settings_getLastReaderResultText(void)
{
	return s_errorBuffer;
}
