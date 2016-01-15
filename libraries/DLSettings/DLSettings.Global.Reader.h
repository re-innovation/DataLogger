#ifndef _DL_SETTINGS_GLOBAL_READER_H_
#define _DL_SETTINGS_GLOBAL_READER_H_

/*
 * Public Functions
 */

void Settings_InitReader(void);

SETTINGS_READER_RESULT Settings_readGlobalFromFile(LocalStorageInterface * pInterface, char const * const filename);

void Settings_requireInt(INTSETTING setting);
void Settings_requireString(STRINGSETTING setting);

bool Settings_allRequiredSettingsRead(void);
void Settings_getMissingNames(char * buffer, uint32_t size);

SETTINGS_READER_RESULT Settings_getLastReaderResult(void);
SETTINGS_READER_RESULT Settings_readFromString(char const * const string, int lineNo);
char const * Settings_getLastReaderResultText(void);

#endif
