#ifndef _DL_SETTINGS_DATACHANNELS_H_
#define _DL_SETTINGS_DATACHANNELS_H_

#define MAX_CHANNELS (32)

typedef uint32_t CHANNELNUMBER;

/*
 * Public Functions
 */
 
void Settings_InitDataChannels(void);

SETTINGS_READER_RESULT Settings_parseDataChannelSetting(char const * const setting, int lineNo);

FIELD_TYPE Settings_GetChannelType(CHANNELNUMBER channel);
bool Settings_ChannelSettingIsValid(CHANNELNUMBER channel);

void * Settings_GetData(CHANNELNUMBER channel);
VOLTAGECHANNEL * Settings_GetDataAsVoltage(CHANNELNUMBER channel);
CURRENTCHANNEL * Settings_GetDataAsCurrent(CHANNELNUMBER channel);

uint32_t Settings_GetMaxChannels(void);

#endif
