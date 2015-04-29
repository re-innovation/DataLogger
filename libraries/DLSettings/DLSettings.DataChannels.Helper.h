#ifndef _DL_SETTINGS_DATACHANNELS_HELPER_H_
#define _DL_SETTINGS_DATACHANNELS_HELPER_H_

/*
 * Public Functions
 */

int8_t Settings_getChannelFromSetting(char const * const setting);
bool Setting_getChannelSettingStr(char * buffer, char const * const setting);
FIELD_TYPE Setting_parseSettingAsType(char const * const setting);
bool Setting_parseSettingAsInt(int32_t * pResult, char const * const setting);
bool Setting_parseSettingAsFloat(float * pResult, char const * const setting);

#endif
