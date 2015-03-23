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

#include <iostream>

/*
 * Local Includes
 */

#include "DLSettings.h"
#include "DLSettings.Reader.h"

/*
 * Private Functions
 */

bool addIntSettingFromString(INTSETTING i, char const * const pValue)
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

/*
 * Public Functions
 */

bool Settings_ReadFromString(char const * const string)
{
	if (!string) { return false; }

	// Find the equals sign (separates name from value)
	char const * const pEquals = strchr((char*)string, '=');

	// If there is no equals sign, fail early
    if (!pEquals) {return false; }

	// The length of the setting name is the difference between the two pointers
	int settingNameLength = pEquals - string;

	// Try to find int setting
	char const * pSettingName;
	uint8_t i;
	for (i = 0; i < INT_SETTINGS_COUNT; ++i)
	{
		pSettingName = Settings_getIntName((INTSETTING)i);
		if (0 == strncmp(pSettingName, string, settingNameLength))
		{
			// As soo as the setting name has been found, try parsing the int and return
			return addIntSettingFromString((INTSETTING)i, pEquals+1);
		}
	}


	// Searching for ints didn't work, try to find string setting
	for (i = 0; i < STRING_SETTINGS_COUNT; ++i)
	{
		pSettingName = Settings_getStringName((STRINGSETTING)i);
		if (0 == strncmp(pSettingName, string, settingNameLength))
		{
			// As soon as the setting name has been found, set the setting and return
			Settings_setString((STRINGSETTING)i, pEquals+1);
			return true;
		}
	}

	// If execution got this far, the setting name wasn't found.
	return false;
}
