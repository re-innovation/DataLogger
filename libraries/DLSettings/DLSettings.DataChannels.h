#ifndef _DL_SETTINGS_DATACHANNELS_H_
#define _DL_SETTINGS_DATACHANNELS_H_

#define MAX_CHANNELS (32)

typedef uint32_t CHANNELNUMBER;

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

DATACHANNELERROR Settings_parseDataChannelSetting(char const * const setting);

FIELD_TYPE Settings_GetChannelType(CHANNELNUMBER channel);
bool Settings_ChannelSettingIsValid(CHANNELNUMBER channel);

void * Settings_GetData(uint8_t channel);
VOLTAGECHANNEL * Settings_GetDataAsVoltage(uint8_t channel);
CURRENTCHANNEL * Settings_GetDataAsCurrent(uint8_t channel);

uint32_t Settings_GetMaxChannels(void);

#endif
