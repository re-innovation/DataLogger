#ifndef _DL_SETTINGS_DATACHANNELS_H_
#define _DL_SETTINGS_DATACHANNELS_H_

#define MAX_CHANNELS (32)

/*
 * List the possible errors that can result from parsing a string
 */

enum datachannelerror
{
    ERR_DATA_CH_NONE,
    ERR_DATA_CH_NO_EQUALS,
    ERR_DATA_CH_NO_CHANNEL,
    ERR_DATA_CH_NO_SETTING,
    ERR_DATA_CH_UNKNOWN_TYPE,
    ERR_DATA_CH_UNKNOWN_SETTING,
    ERR_DATA_CH_INVALID_SETTING,
    ERR_DATA_CH_CHANNEL_TYPE_NOT_SET,
};
typedef enum datachannelerror DATACHANNELERROR;

/*
 * Public Functions
 */
 
void Settings_InitDataChannels(void);

void Settings_SetupAllValidChannels(DataFieldManager * pManager);
DATACHANNELERROR Settings_parseDataChannelSetting(char const * const setting);

FIELD_TYPE Settings_GetChannelType(uint8_t channel);
bool Settings_ChannelSettingIsValid(uint8_t channel);

VOLTAGECHANNEL * Settings_GetDataAsVoltage(uint8_t channel);
CURRENTCHANNEL * Settings_GetDataAsCurrent(uint8_t channel);

#endif
