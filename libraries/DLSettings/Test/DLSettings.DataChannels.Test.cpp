/*
 * DLSettings.DataChannels.Test.cpp
 *
 * Tests the DataChannels settings reader
 *
 * Author: James Fowkes
 *
 * www.re-innovation.co.uk
 */

/*
 * C++ Library Includes
 */

#include <stdint.h>
#include <string.h>

/*
 * Local Application Includes
 */

#include "../DLSettings.h"
 
/*
 * Unity Test Framework
 */

#include "unity.h"

void setUp(void)
{
    Settings_Init();
}

void test_MinusOneIsReturnedFromInvalidChannelNumber(void)
{
    TEST_ASSERT_EQUAL(-1, Settings_getChannelFromSetting("Channel")); // No channel number
    TEST_ASSERT_EQUAL(-1, Settings_getChannelFromSetting("Chanel0")); // Misspelt "channel"
    TEST_ASSERT_EQUAL(-1, Settings_getChannelFromSetting("Channel-3")); // Negative number
}

void test_CorrectChannelNumberIsReturnedFromSetting(void)
{
    TEST_ASSERT_EQUAL(0, Settings_getChannelFromSetting("Channel0")); 
    TEST_ASSERT_EQUAL(1, Settings_getChannelFromSetting("channel1"));
    TEST_ASSERT_EQUAL(2, Settings_getChannelFromSetting("cHannel2"));
    TEST_ASSERT_EQUAL(3, Settings_getChannelFromSetting("CH3")); // CH can be used as shorthand for channel
}

void test_CorrectChannelTypeIsMatchedFromSetting(void)
{
    char buffer[10];
    char expected1[] = "type";
    TEST_ASSERT_TRUE(Setting_getChannelSettingStr(buffer, "Channel1.type"));
    TEST_ASSERT_EQUAL_STRING(expected1, buffer);

    TEST_ASSERT_TRUE(Setting_getChannelSettingStr(buffer, "Channel2.Type"));
    TEST_ASSERT_EQUAL_STRING(expected1, buffer);

    char expected2[] = "mvperamp";
    TEST_ASSERT_TRUE(Setting_getChannelSettingStr(buffer, "Channel2.mvPerAmp"));
    TEST_ASSERT_EQUAL_STRING(expected2, buffer);
}

void test_CorrectTypeIsParsedFromSetting(void)
{
    TEST_ASSERT_EQUAL(VOLTAGE, Setting_parseSettingAsType("Voltage"));
    TEST_ASSERT_EQUAL(CURRENT, Setting_parseSettingAsType("current"));
}

void test_CorrectIntIsParsedFromSetting(void)
{
    int32_t result;
    TEST_ASSERT_TRUE(Setting_parseSettingAsInt(&result, "10"));
    TEST_ASSERT_EQUAL(10, result);

    TEST_ASSERT_TRUE(Setting_parseSettingAsInt(&result, "-10"));
    TEST_ASSERT_EQUAL(-10, result);

    TEST_ASSERT_FALSE(Setting_parseSettingAsInt(&result, "NOTANINT"));
}

void test_CorrectFloatIsParsedFromSetting(void)
{
    float result;
    TEST_ASSERT_TRUE(Setting_parseSettingAsFloat(&result, "1.234"));
    TEST_ASSERT_EQUAL_FLOAT(1.234, result);

    TEST_ASSERT_TRUE(Setting_parseSettingAsFloat(&result, "-37.324"));
    TEST_ASSERT_EQUAL_FLOAT(-37.324, result);

    TEST_ASSERT_FALSE(Setting_parseSettingAsFloat(&result, "NOTAFLOAT"));
}

void test_NoChannelInSettingReturnsNoChannelError(void)
{
    TEST_ASSERT_EQUAL(ERR_DATA_CH_NO_CHANNEL, Settings_parseDataChannelSetting("Channel.Type = Voltage"));
}

void test_InvalidChannelInSettingReturnsNoChannelError(void)
{
    TEST_ASSERT_EQUAL(ERR_DATA_CH_NO_CHANNEL, Settings_parseDataChannelSetting("Channel-1.Type = Voltage"));
}

void test_NoSettingInSettingReturnsNoSettingError(void)
{
    TEST_ASSERT_EQUAL(ERR_DATA_CH_NO_SETTING, Settings_parseDataChannelSetting("Channel1 = Voltage"));   
}

void test_NoEqualsInSettingReturnsNoEqualsError(void)
{
    TEST_ASSERT_EQUAL(ERR_DATA_CH_NO_EQUALS, Settings_parseDataChannelSetting("Channel1.Type - Voltage"));
}

void test_UnknownSettingInSettingReturnsUnknownSettingError(void)
{
    // Channel must have type set in order to try parsing other settings
    TEST_ASSERT_EQUAL(ERR_DATA_CH_NONE, Settings_parseDataChannelSetting("Channel1.Type = Voltage"));
    TEST_ASSERT_EQUAL(ERR_DATA_CH_UNKNOWN_SETTING, Settings_parseDataChannelSetting("Channel1.BadSetting = Voltage"));
}

void test_UnknownTypeForTypeReturnsUnknownTypeError(void)
{
    TEST_ASSERT_EQUAL(ERR_DATA_CH_UNKNOWN_TYPE, Settings_parseDataChannelSetting("Channel1.Type = Woltage"));
}

void test_ValidSettingWithoutChannelTypeReturnChannelTypeNotSetError(void)
{
    TEST_ASSERT_EQUAL(ERR_DATA_CH_CHANNEL_TYPE_NOT_SET, Settings_parseDataChannelSetting("Channel1.mvPerA = 234"));
}

void test_ValidVoltageSettingsAreParsedCorrectly(void)
{
    TEST_ASSERT_EQUAL(ERR_DATA_CH_NONE, Settings_parseDataChannelSetting("CH1.Type =Voltage"));
    TEST_ASSERT_EQUAL(VOLTAGE, Settings_GetChannelType(1));
    TEST_ASSERT_FALSE(Settings_ChannelSettingIsValid(1));

    TEST_ASSERT_EQUAL(ERR_DATA_CH_NONE, Settings_parseDataChannelSetting("cH1.r1= 18300.0"));
    TEST_ASSERT_FALSE(Settings_ChannelSettingIsValid(1));

    TEST_ASSERT_EQUAL(ERR_DATA_CH_NONE, Settings_parseDataChannelSetting("ch1.R2 = 10000.0"));
    TEST_ASSERT_TRUE(Settings_ChannelSettingIsValid(1));

    TEST_ASSERT_EQUAL_FLOAT(18300.0, Settings_GetDataAsVoltage(1)->R1);
    TEST_ASSERT_EQUAL_FLOAT(10000.0, Settings_GetDataAsVoltage(1)->R2);
}

void test_ValidCurrentSettingsAreParsedCorrectly(void)
{
    TEST_ASSERT_EQUAL(ERR_DATA_CH_NONE, Settings_parseDataChannelSetting("ch1.Type =Current"));
    TEST_ASSERT_EQUAL(CURRENT, Settings_GetChannelType(1));
    TEST_ASSERT_FALSE(Settings_ChannelSettingIsValid(1));

    TEST_ASSERT_EQUAL(ERR_DATA_CH_NONE, Settings_parseDataChannelSetting("channel1.mvperAmp= 400"));
    TEST_ASSERT_FALSE(Settings_ChannelSettingIsValid(1));

    TEST_ASSERT_EQUAL(ERR_DATA_CH_NONE, Settings_parseDataChannelSetting("channel1.offset = 12"));
    TEST_ASSERT_TRUE(Settings_ChannelSettingIsValid(1));

    TEST_ASSERT_EQUAL_FLOAT(400.0, Settings_GetDataAsCurrent(1)->mvPerAmp);
    TEST_ASSERT_EQUAL_FLOAT(12.0, Settings_GetDataAsCurrent(1)->offset);
}

int main(void)
{
    UnityBegin("DLSettings.DataChannels.Test.cpp");

    RUN_TEST(test_MinusOneIsReturnedFromInvalidChannelNumber);
  	RUN_TEST(test_CorrectChannelNumberIsReturnedFromSetting);
    RUN_TEST(test_CorrectChannelTypeIsMatchedFromSetting);
    RUN_TEST(test_CorrectTypeIsParsedFromSetting);
    RUN_TEST(test_CorrectIntIsParsedFromSetting);
    RUN_TEST(test_CorrectFloatIsParsedFromSetting);

    RUN_TEST(test_NoChannelInSettingReturnsNoChannelError);
    RUN_TEST(test_InvalidChannelInSettingReturnsNoChannelError);
    RUN_TEST(test_NoSettingInSettingReturnsNoSettingError);
    RUN_TEST(test_NoEqualsInSettingReturnsNoEqualsError);
    RUN_TEST(test_UnknownSettingInSettingReturnsUnknownSettingError);
    RUN_TEST(test_UnknownTypeForTypeReturnsUnknownTypeError);
    RUN_TEST(test_ValidSettingWithoutChannelTypeReturnChannelTypeNotSetError);

    RUN_TEST(test_ValidVoltageSettingsAreParsedCorrectly);
    RUN_TEST(test_ValidCurrentSettingsAreParsedCorrectly);

  	UnityEnd();
  	return 0;
}

