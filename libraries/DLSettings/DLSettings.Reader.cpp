/*
 * DLSettings.Reader.cpp
 *
 * Sets settings based on lines read from config file or similar
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
#include <ctype.h>

/*
 * Local Includes
 */

#include "DLSettings.h"
#include "DLUtility.h"

/*
 * Private Variables
 */

static char const * const s_errorStrings[] = {
	"", 
	ERROR_STR_NO_STRING,
	ERROR_STR_NO_EQUALS,
	ERROR_STR_NO_NAME,
	ERROR_STR_INVALID_INT,
};

static char s_errorBuffer[100];
static SETTINGS_READER_RESULT s_lastResult = ERR_READER_NONE;

static bool s_intSettingIsRequired[INT_SETTINGS_COUNT];
static bool s_stringSettingIsRequired[INT_SETTINGS_COUNT];

/*
 * Private Functions
 */

static bool addIntSettingFromString(INTSETTING i, char const * const pValue)
{
	// strtol will set endOfConversion to > pValue if conversion succeeded
	// or == pValue if it failed
	char * endOfConversion = NULL;

	int value = (int)strtol(pValue, &endOfConversion, 10);

	if (endOfConversion == pValue)
	{
		// No valid conversion could be performed
		return false;
	}

	Settings_setInt(i, value);
	return true;
}

static SETTINGS_READER_RESULT noStringError(void)
{
	s_lastResult = ERR_READER_NO_STRING;
	sprintf(s_errorBuffer, s_errorStrings[ERR_READER_NO_STRING]);
	return s_lastResult;
}

static SETTINGS_READER_RESULT noEqualsError(void)
{
	s_lastResult = ERR_READER_NO_EQUALS;
	sprintf(s_errorBuffer, s_errorStrings[ERR_READER_NO_EQUALS]);
	return s_lastResult;
}

static SETTINGS_READER_RESULT noNameError(char * pName)
{
	s_lastResult = ERR_READER_NO_NAME;
	sprintf(s_errorBuffer, s_errorStrings[ERR_READER_NO_NAME], pName);
	return s_lastResult;
}

static SETTINGS_READER_RESULT invalidIntError(char * pName, char * pSetting)
{
	s_lastResult = ERR_READER_INVALID_INT;
	sprintf(s_errorBuffer, s_errorStrings[ERR_READER_INVALID_INT], pSetting, pName);
	return s_lastResult;
}

static SETTINGS_READER_RESULT noError(void)
{
	s_lastResult = ERR_READER_NONE;
	s_errorBuffer[0] = '\0';
	return s_lastResult;
}

/*
 * Public Functions
 */

SETTINGS_READER_RESULT Settings_readFromString(char const * const string)
{
	char settingNameCopy[64];
	char settingValueCopy[64];

	if (!string) { return noStringError(); }

	// If line is blank, fail early
    if (stringIsWhitespace(string)) { return noError(); }

	// If line is a comment, skip immediately
	if (*string == '#') { return noError(); }

	// Find the equals sign (separates name from value)
	char * pEndOfName;
	char * pStartOfSetting;

	if (!splitAndStripWhiteSpace((char*)string, '=', NULL, &pEndOfName, &pStartOfSetting, NULL))
	{
		return noEqualsError();
	}

	// The length of the setting name is the difference between the two pointers
	int settingNameLength = pEndOfName - string + 1;
	strncpy_safe(settingNameCopy, string, settingNameLength+1);
	strncpy_safe(settingValueCopy, pStartOfSetting, 30);

	// Try to find int setting
	char const * pSettingName;
	uint8_t i;
	for (i = 0; i < INT_SETTINGS_COUNT; ++i)
	{
		pSettingName = Settings_getIntName((INTSETTING)i);
		if (0 == strcmp(pSettingName, settingNameCopy))
		{
			// As soon as the setting name has been found, try parsing the int and return.
			bool result = addIntSettingFromString((INTSETTING)i, settingValueCopy);
			if (!result)
			{
				return invalidIntError(settingNameCopy, settingValueCopy);
			}
			return noError();
		}
	}


	// Searching for ints didn't work, try to find string setting
	for (i = 0; i < STRING_SETTINGS_COUNT; ++i)
	{
		pSettingName = Settings_getStringName((STRINGSETTING)i);
		if (0 == strcmp(pSettingName, settingNameCopy))
		{
			// As soon as the setting name has been found, set the setting and return
			Settings_setString((STRINGSETTING)i, settingValueCopy);
			return noError();
		}
	}

	// If execution got this far, the setting name wasn't found.
	return noNameError(settingNameCopy);
}

SETTINGS_READER_RESULT Settings_getLastReaderResult(void)
{
	return s_lastResult;
}

char const * Settings_getLastReaderResultText(void)
{
	return s_errorBuffer;
}

void Settings_InitReader(void)
{
	s_lastResult = ERR_READER_NONE;
	s_errorBuffer[0] = '\0';
}

void Settings_requireInt(INTSETTING setting)
{
	if (setting < INT_SETTINGS_COUNT) { s_intSettingIsRequired[setting] = true; }
}

void Settings_requireString(STRINGSETTING setting)
{
	if (setting < STRING_SETTINGS_COUNT) { s_stringSettingIsRequired[setting] = true; }
}

bool Settings_allRequiredSettingsRead(void)
{
	bool allRequiredSettingsRead = true;
	uint8_t i;

	for (i = 0; i < INT_SETTINGS_COUNT; ++i)
	{
		if (s_intSettingIsRequired[i])
		{
			allRequiredSettingsRead &= Settings_intIsSet((INTSETTING)i);
		}
	}

	for (i = 0; i < STRING_SETTINGS_COUNT; ++i)
	{
		if (s_stringSettingIsRequired[i])
		{
			allRequiredSettingsRead &= Settings_stringIsSet((STRINGSETTING)i);
		}
	}

	return allRequiredSettingsRead;
}

void Settings_getMissingNames(char * buffer, uint32_t size)
{
	uint8_t i;
	FixedLengthAccumulator accum(buffer, size);

	for (i = 0; i < INT_SETTINGS_COUNT; ++i)
	{
		if (s_intSettingIsRequired[i] & !Settings_intIsSet((INTSETTING)i))
		{
			accum.writeString(Settings_getIntName((INTSETTING)(i)));
			accum.writeString(", "); 
		}
	}

	for (i = 0; i < STRING_SETTINGS_COUNT; ++i)
	{
		if (s_stringSettingIsRequired[i] & !Settings_stringIsSet((STRINGSETTING)i))
		{
			accum.writeString(Settings_getStringName((STRINGSETTING)(i)));
			accum.writeString(", "); 
		}
	}

	accum.remove(2); // Remove last ", "
}
