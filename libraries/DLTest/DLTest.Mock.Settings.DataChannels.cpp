/*
 * C++ Library Includes
 */

#include <stdint.h>

#include <iostream>
#include <fstream>

#include "DLDataField.Types.h"
#include "DLSettings.DataChannels.h"

static VOLTAGECHANNEL s_voltageChannelSettings = {
	.mvPerBit = 0.125f,
	.R1 = 200000.0f,
	.R2= 10000.0f,
};

static CURRENTCHANNEL s_currentChannelSettings = {
	.mvPerBit = 0.125f,
	.offset = 60.0f,
	.mvPerAmp = 600.0f,
};


void Settings_InitDataChannels(void)
{
    
}

DATACHANNELERROR Settings_parseDataChannelSetting(char const * const setting)
{
    (void)setting; return ERR_DATA_CH_NONE;
}

FIELD_TYPE Settings_GetChannelType(uint8_t channel)
{
    (void)channel; return VOLTAGE;
}

bool Settings_ChannelSettingIsValid(uint8_t channel)
{
    (void)channel; return true;
}

void * Settings_GetData(uint8_t channel)
{
    (void)channel; return (void*)&s_voltageChannelSettings;
}

VOLTAGECHANNEL * Settings_GetDataAsVoltage(uint8_t channel)
{
    (void)channel; return &s_voltageChannelSettings;
}

CURRENTCHANNEL * Settings_GetDataAsCurrent(uint8_t channel)
{
    (void)channel; return &s_currentChannelSettings;
}

uint32_t Settings_GetMaxChannels(void)
{
    return 1;
}
