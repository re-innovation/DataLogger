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

typedef void LocalStorageInterface;
typedef void DataFieldManager;

#include "DLDataField.Types.h"
#include "DLSettings.h"
#include "DLSettings.Global.h"
#include "DLSettings.Reader.h"
#include "DLSettings.Reader.Errors.h"
#include "DLUtility.h"

/*
 * Public Functions
 */

SETTINGS_READER_RESULT Settings_readGlobalFromFile(LocalStorageInterface * pInterface, char const * const filename)
{
	(void)pInterface; (void)filename; return ERR_READER_NONE;
}

/*
 * Settings_readChannelsFromFile
 *
 * Read datachannel settings from the specified file
 * and configure the datafield manager
 *
 * pManager - pointer to the application datafield manager
 * pInterface - pointer to the application storage interface
 * filename - filename to read
 */
SETTINGS_READER_RESULT Settings_readChannelsFromFile(
	DataFieldManager * pManager, LocalStorageInterface * pInterface, char const * const filename)
{
    (void)pManager; (void)pInterface; (void)filename; return ERR_READER_NONE;
}

SETTINGS_READER_RESULT Settings_readFromString(char const * const string)
{
	(void)string; return ERR_READER_NONE;
}

SETTINGS_READER_RESULT Settings_getLastReaderResult(void)
{
	return ERR_READER_NONE;
}

char const * Settings_getLastReaderResultText(void)
{
	return "";
}

void Settings_InitReader(void) {}

void Settings_requireInt(INTSETTING setting) { (void)setting; }

void Settings_requireString(STRINGSETTING setting) { (void)setting; }

bool Settings_allRequiredSettingsRead(void)
{
	return true;
}

void Settings_getMissingNames(char * buffer, uint32_t size) { (void)buffer; (void)size; }
