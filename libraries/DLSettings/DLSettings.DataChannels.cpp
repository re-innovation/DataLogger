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

#ifdef TEST
#include <iostream>
#endif
 
/*
 * Local Includes
 */

#include "DLDataField.Types.h"
#include "DLSettings.DataChannels.h"
#include "DLSettings.DataChannels.Helper.h"
#include "DLUtility.h"

/*
 * Private Variables
 */

static void * s_channels[MAX_CHANNELS];
static FIELD_TYPE s_fieldTypes[MAX_CHANNELS];

/*
 * For each of the channels that can be stored,
 * the bitfield stores a 1 if a setting has been set.
 * For example, the VOLTAGECHANNEL needs mvPerBit (0x01),
 * R1 (0x02) and R2 (0x04) settings.
 * 
 * If only R1 and mvPerBit are set, the value in the bitfield will be
 * 0x03, not 0x07 and the field will not be considered complete.
 */
static uint8_t s_valuesSetBitFields[MAX_CHANNELS];

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
    s_valuesSetBitFields[ch] = 0;
    switch(type)    
    {
    case VOLTAGE:
        s_channels[ch] = new VOLTAGECHANNEL();
        break;
    case CURRENT:
        s_channels[ch] = new CURRENTCHANNEL();
    case TEMPERATURE_C:
    case TEMPERATURE_F:
    case TEMPERATURE_K:
        s_channels[ch] = new THERMISTORCHANNEL();
        break;
    default:
    case INVALID_TYPE:
        break;
    }
}

static bool voltageChannelIsValid(uint8_t channel)
{
    return s_valuesSetBitFields[channel] == 0x07; // Voltage needs three values set   
}

static bool currentChannelIsValid(uint8_t channel)
{
    return s_valuesSetBitFields[channel] == 0x07; // Current needs three values set   
}

static bool thermistorChannelIsValid(uint8_t channel)
{
    return s_valuesSetBitFields[channel] == 0x0F; // Thermistor needs four values set   
}

static DATACHANNELERROR tryParseAsVoltageSetting(uint8_t ch, char * pSettingName, char * pValueString)
{
    float setting;

    bool settingParsedAsFloat = Setting_parseSettingAsFloat(&setting, pValueString);

    if (0 == strncmp(pSettingName, "mvperbit", 8))
    {
        if (!settingParsedAsFloat) { return invalidSettingError(); }
        ((VOLTAGECHANNEL*)s_channels[ch])->mvPerBit = setting;
        s_valuesSetBitFields[ch] |= 0x01;
        return noError();
    }

    if (0 == strncmp(pSettingName, "r1", 2))
    {
        if (!settingParsedAsFloat) { return invalidSettingError(); }
        ((VOLTAGECHANNEL*)s_channels[ch])->R1 = setting;
        s_valuesSetBitFields[ch] |= 0x02;
        return noError();
    }

    if (0 == strncmp(pSettingName, "r2", 2))
    {
        if (!settingParsedAsFloat) { return invalidSettingError(); }
        ((VOLTAGECHANNEL*)s_channels[ch])->R2 = setting;
        s_valuesSetBitFields[ch] |= 0x04;
        return noError();
    }

    return unknownSettingError();
}

static DATACHANNELERROR tryParseAsCurrentSetting(uint8_t ch, char * pSettingName, char * pValueString)
{
    float setting;
    
    bool settingParsedAsFloat = Setting_parseSettingAsFloat(&setting, pValueString);
    
    if (0 == strncmp(pSettingName, "mvperbit", 8))
    {
        if (!settingParsedAsFloat) { return invalidSettingError(); }
        ((CURRENTCHANNEL*)s_channels[ch])->mvPerBit = setting;
        s_valuesSetBitFields[ch] |= 0x01;
        return noError();
    }
    
    if (0 == strncmp(pSettingName, "offset", 6))
    {
        if (!settingParsedAsFloat) { return invalidSettingError(); }
        ((CURRENTCHANNEL*)s_channels[ch])->offset = setting;
        s_valuesSetBitFields[ch] |= 0x02;
        return noError();
    }

    if (0 == strncmp(pSettingName, "mvperamp", 8))
    {
        if (!settingParsedAsFloat) { return invalidSettingError(); }
        ((CURRENTCHANNEL*)s_channels[ch])->mvPerAmp = setting;
        s_valuesSetBitFields[ch] |= 0x04;
        return noError();
    }

    return unknownSettingError();
}

static DATACHANNELERROR tryParseAsThermistorSetting(uint8_t ch, char * pSettingName, char * pValueString)
{
    float setting;
    
    bool settingParsedAsFloat = Setting_parseSettingAsFloat(&setting, pValueString);
    
    if (0 == strncmp(pSettingName, "maxadc", 6))
    {
        if (!settingParsedAsFloat) { return invalidSettingError(); }
        ((THERMISTORCHANNEL*)s_channels[ch])->maxADC = setting;
        s_valuesSetBitFields[ch] |= 0x01;
        return noError();
    }
    
    if (0 == strncmp(pSettingName, "b", 1))
    {
        if (!settingParsedAsFloat) { return invalidSettingError(); }
        ((THERMISTORCHANNEL*)s_channels[ch])->B = setting;
        s_valuesSetBitFields[ch] |= 0x02;
        return noError();
    }

    if (0 == strncmp(pSettingName, "r25", 3))
    {
        if (!settingParsedAsFloat) { return invalidSettingError(); }
        ((THERMISTORCHANNEL*)s_channels[ch])->R25 = setting;
        s_valuesSetBitFields[ch] |= 0x04;
        return noError();
    }

    if (0 == strncmp(pSettingName, "otherr", 6))
    {
        if (!settingParsedAsFloat) { return invalidSettingError(); }
        ((THERMISTORCHANNEL*)s_channels[ch])->otherR = setting;
        s_valuesSetBitFields[ch] |= 0x08;
        return noError();
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
        s_channels[i] = NULL;
        s_valuesSetBitFields[i] = 0x00;
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

    if (*setting == '#') { return noError(); } // Line is a comment

    // Split the string by '=' to get setting and name
    success &= splitAndStripWhiteSpace(lowercaseCopy, '=', &pSettingString, NULL, &pValueString, NULL);
    if (!success) { return noEqualsError(); }

    // Split the setting to get channel and setting name
    success &= splitAndStripWhiteSpace(pSettingString, '.', &pChannelString, NULL, &pChannelSettingString, NULL);
    if (!success) { return noSettingError(); }

    int8_t ch = Settings_getChannelFromSetting(pChannelString);
    if (ch == -1) { return noChannelError(); }

    
    if (0 == strncmp(pChannelSettingString, "type", 4))
    {
        // Try to interpret setting as a channel type
        s_fieldTypes[ch] = Setting_parseSettingAsType(pValueString);
        if (s_fieldTypes[ch] == INVALID_TYPE) { return unknownTypeError(); }

        setupChannel(ch, s_fieldTypes[ch]);
        return noError();
    }

    /* If processing got this far, the setting needs to be interpreted based on the channel datatype */
    switch (s_fieldTypes[ch])
    {
    case VOLTAGE:
        return tryParseAsVoltageSetting(ch, pChannelSettingString, pValueString);
    case CURRENT:
        return tryParseAsCurrentSetting(ch, pChannelSettingString, pValueString);
    case TEMPERATURE_C:
    case TEMPERATURE_F:
    case TEMPERATURE_K:
        return tryParseAsThermistorSetting(ch, pChannelSettingString, pValueString);

    case INVALID_TYPE:
    default:
        // If the channel type is not set prior to any other settings, this is an error.
        return channelNotSetError();
    }
}

FIELD_TYPE Settings_GetChannelType(CHANNELNUMBER channel)
{
    if (channel > MAX_CHANNELS || channel == 0) { return INVALID_TYPE; }
    return s_fieldTypes[channel-1];
}

bool Settings_ChannelSettingIsValid(CHANNELNUMBER channel)
{
    if (channel > MAX_CHANNELS || channel == 0) { return false; }

    channel--; // Switch from one- to zero-indexing

    if (s_fieldTypes[channel] == INVALID_TYPE) { return false; }

    switch(s_fieldTypes[channel])
    {
    case VOLTAGE:
        return voltageChannelIsValid(channel);
    case CURRENT:
        return currentChannelIsValid(channel);
    case TEMPERATURE_C:
    case TEMPERATURE_F:
    case TEMPERATURE_K:
        return thermistorChannelIsValid(channel);
    default:
        return false;
    }
}

void * Settings_GetData(CHANNELNUMBER channel)
{
    if (channel > MAX_CHANNELS || channel == 0) { return NULL; }
    channel--; // Switch from one- to zero-indexing
    return s_channels[channel];
}

VOLTAGECHANNEL * Settings_GetDataAsVoltage(CHANNELNUMBER channel)
{
    if (channel > MAX_CHANNELS || channel == 0) { return NULL; }
    channel--; // Switch from one- to zero-indexing
    return (VOLTAGECHANNEL*)s_channels[channel];
}

CURRENTCHANNEL * Settings_GetDataAsCurrent(CHANNELNUMBER channel)
{
    if (channel > MAX_CHANNELS || channel == 0) { return NULL; }
    channel--; // Switch from one- to zero-indexing
    return (CURRENTCHANNEL*)s_channels[channel];
}

uint32_t Settings_GetMaxChannels(void)
{
    return (uint32_t)MAX_CHANNELS;
}
