/*
 * DLSettings.DataChannels.cpp
 * 
 * Manages reading and storing per-channel settings
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
#include <ctype.h>
#include <stdio.h>

/*
 * Local Includes
 */

#include "DLLocalStorage.h"
#include "DLDataField.Types.h"
#include "DLSettings.DataChannels.h"
#include "DLSettings.DataChannels.Helper.h"
#include "DLUtility.h"

/*
 * Private Variables
 */

static CHANNEL s_channels[MAX_CHANNELS];

static DATACHANNELERROR s_lastResult = ERR_DATA_CH_NONE;

/*
 * Private Functions
 */

static DATACHANNELERROR noError(void)
{
    s_lastResult = ERR_DATA_CH_NONE;
    return s_lastResult;
}

static DATACHANNELERROR noEqualsError(void)
{
    s_lastResult = ERR_DATA_CH_NO_EQUALS;
    return s_lastResult;
}

static DATACHANNELERROR noChannelError(void)
{
    s_lastResult = ERR_DATA_CH_NO_CHANNEL;
    return s_lastResult;
}

static DATACHANNELERROR noSettingError(void)
{
    s_lastResult = ERR_DATA_CH_NO_SETTING;
    return s_lastResult;  
}

static DATACHANNELERROR unknownTypeError(void)
{
    s_lastResult = ERR_DATA_CH_UNKNOWN_TYPE;
    return s_lastResult;  
}

static DATACHANNELERROR unknownSettingError(void)
{
    s_lastResult = ERR_DATA_CH_UNKNOWN_SETTING;
    return s_lastResult;
}

static DATACHANNELERROR invalidSettingError(void)
{
    s_lastResult = ERR_DATA_CH_INVALID_SETTING;
    return s_lastResult;
}

static DATACHANNELERROR channelNotSetError(void)
{
    s_lastResult = ERR_DATA_CH_CHANNEL_TYPE_NOT_SET;
    return s_lastResult;  
}

static void setupChannel(uint8_t ch, FIELD_TYPE type)
{
    s_channels[ch].type = type;
    switch(type)    
    {
    case VOLTAGE:
        s_channels[ch].data = new VOLTAGECHANNEL();
        ((VOLTAGECHANNEL*)s_channels[ch].data)->valuesSet[0] = false;
        ((VOLTAGECHANNEL*)s_channels[ch].data)->valuesSet[1] = false;
        break;
    case CURRENT:
        s_channels[ch].data = new CURRENTCHANNEL();
        ((CURRENTCHANNEL*)s_channels[ch].data)->valuesSet[0] = false;
        ((CURRENTCHANNEL*)s_channels[ch].data)->valuesSet[1] = false;
        break;
    default:
    case INVALID_TYPE:
        break;
    }
}

static bool voltageChannelIsValid(VOLTAGECHANNEL * channelData)
{
    return channelData->valuesSet[0] && channelData->valuesSet[1];
}

static bool currentChannelIsValid(CURRENTCHANNEL * channelData)
{
    return channelData->valuesSet[0] && channelData->valuesSet[1];
}

static DATACHANNELERROR tryParseAsVoltageSetting(uint8_t ch, char * pSettingName, char * pValueString)
{
    float resistance;
    if (0 == strncmp(pSettingName, "r1", 2))
    {
        if (Setting_parseSettingAsFloat(&resistance, pValueString))
        {
            ((VOLTAGECHANNEL*)s_channels[ch].data)->R1 = resistance;
            ((VOLTAGECHANNEL*)s_channels[ch].data)->valuesSet[0] = true;
            return noError();
        }
    }


    if (0 == strncmp(pSettingName, "r2", 2))
    {
        if (Setting_parseSettingAsFloat(&resistance, pValueString))
        {
            ((VOLTAGECHANNEL*)s_channels[ch].data)->R2 = resistance;
            ((VOLTAGECHANNEL*)s_channels[ch].data)->valuesSet[1] = true;
            return noError();
        }
    }

    return unknownSettingError();
}

static DATACHANNELERROR tryParseAsCurrentSetting(uint8_t ch, char * pSettingName, char * pValueString)
{
    float value;
    if (0 == strncmp(pSettingName, "offset", 6))
    {
        if (Setting_parseSettingAsFloat(&value, pValueString))
        {
            ((CURRENTCHANNEL*)s_channels[ch].data)->offset = value;
            ((CURRENTCHANNEL*)s_channels[ch].data)->valuesSet[0] = true;
            return noError();
        }
        else
        {
            return invalidSettingError();
        }
    }

    if (0 == strncmp(pSettingName, "mvperamp", 8))
    {
        if (Setting_parseSettingAsFloat(&value, pValueString))
        {
            ((CURRENTCHANNEL*)s_channels[ch].data)->mvPerAmp = value;
            ((CURRENTCHANNEL*)s_channels[ch].data)->valuesSet[1] = true;
            return noError();
        }
        else
        {
            return invalidSettingError();
        }
    }

    return unknownSettingError();
}

/*
 * Public Functions
 */

void Settings_InitDataChannels(void)
{
    uint8_t i;
    for (i = 0; i < MAX_CHANNELS; ++i)
    {
        s_channels[i].type = INVALID_TYPE;
        s_channels[i].data = NULL;
    }
}

DATACHANNELERROR Settings_parseDataChannelSetting(char const * const setting)
{
    char * pSettingString;
    char * pValueString;
    char * pChannelString;
    char * pChannelSettingString;

    uint8_t length = strlen(setting);

    char * lowercaseCopy = new char[length+1];
    strncpy_safe(lowercaseCopy, setting, length+1);
    toLowerStr(lowercaseCopy);

    bool success = true;

    // Split the string by '=' to get setting and name
    success &= splitAndStripWhiteSpace((char*)lowercaseCopy, '=', &pSettingString, NULL, &pValueString, NULL);
    if (!success) { return noEqualsError(); }

    // Split the setting to get channel and setting name
    success &= splitAndStripWhiteSpace(pSettingString, '.', &pChannelString, NULL, &pChannelSettingString, NULL);
    if (!success) { return noSettingError(); }

    int8_t ch = Settings_getChannelFromSetting(pChannelString);
    if (ch == -1) { return noChannelError(); }

    
    if (0 == strncmp(pChannelSettingString, "type", 4))
    {
        // Try to interpret setting as a channel type
        FIELD_TYPE type = Setting_parseSettingAsType(pValueString);
        if (type == INVALID_TYPE) { return unknownTypeError(); }

        setupChannel(ch, type);
        return noError();
    }

    /* If processing got this far, the setting needs to be interpreted based on the channel datatype */
    switch (s_channels[ch].type)
    {
    case VOLTAGE:
        return tryParseAsVoltageSetting(ch, pChannelSettingString, pValueString);
    case CURRENT:
        return tryParseAsCurrentSetting(ch, pChannelSettingString, pValueString);
    case INVALID_TYPE:
    default:
        // If the channel type is not set prior to any other settings, this is an error.
        return channelNotSetError();
    }
}

FIELD_TYPE Settings_GetChannelType(uint8_t channel)
{
    return (channel < MAX_CHANNELS) ? s_channels[channel].type : INVALID_TYPE;
}

bool Settings_ChannelSettingIsValid(uint8_t channel)
{
    if (s_channels[channel].type == INVALID_TYPE) { return false; }
    if (s_channels[channel].data == NULL) { return false; }

    switch(s_channels[channel].type)
    {
    case VOLTAGE:
        return voltageChannelIsValid((VOLTAGECHANNEL*)s_channels[channel].data);
    case CURRENT:
        return currentChannelIsValid((CURRENTCHANNEL*)s_channels[channel].data);
    default:
        return false;
    }
}

VOLTAGECHANNEL * Settings_GetDataAsVoltage(uint8_t channel)
{
    if (s_channels[channel].type != VOLTAGE) { return NULL; }
    return (VOLTAGECHANNEL*)s_channels[channel].data;
}

CURRENTCHANNEL * Settings_GetDataAsCurrent(uint8_t channel)
{
    if (s_channels[channel].type != CURRENT) { return NULL; }
    return (CURRENTCHANNEL*)s_channels[channel].data;
}