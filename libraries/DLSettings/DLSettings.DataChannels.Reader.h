#ifndef _DL_SETTINGS_DATACHANNEL_READER_H_
#define _DL_SETTINGS_DATACHANNEL_READER_H_

/*
 * Public Functions
 */

SETTINGS_READER_RESULT Settings_readChannelsFromFile(
	DataFieldManager * pManager, LocalStorageInterface * pInterface, char const * const filename);

#endif
