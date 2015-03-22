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

/*
 * Local Includes
 */

#include "DLSettings.h"
#include "DLSettings.Reader.h"

/*
 * Public Functions
 */

bool Settings_ReadFromString(char const * const string)
{
	if (!string) { return false; }

	char * pComma = strchr(string, ',');

	if (!pComma) { return false; }

	int settingNameLength = pComma - string;

	// Try to find int setting
	char * pSettingName;
	INTSETTING i;
	for (i = 0; i < INT_SETTINGS_COUNT; ++i)
	{
		pSettingName = Settings_getIntName(i);
		if (0 == strncmp(pSettingName, string, settingNameLength)) { return addIntSettingFromString(i, string); }
	}

	return true;
}