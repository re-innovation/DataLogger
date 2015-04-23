/*
 * DLSettings.Test.cpp
 * 
 * Tests the Settings class
 *
 * Author: James Fowkes
 *
 * www.re-innovation.co.uk
 */

/*
 * C++ Library Includes
 */

#include <string.h>
#include <stdint.h>
 
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

static void test_GetString_ReturnsNULLIfNotSet(void)
{
	TEST_ASSERT_NULL(Settings_getString((STRINGSETTING)0));
}

static void test_GetInt_ReturnsZeroIfNotSet(void)
{
	TEST_ASSERT_EQUAL(0, Settings_getInt((INTSETTING)0));
}

static void test_isSetReturnsCorrectValue(void)
{
	TEST_ASSERT_FALSE(Settings_intIsSet((INTSETTING)0));
	Settings_setInt((INTSETTING)0, 10);
	TEST_ASSERT_TRUE(Settings_intIsSet((INTSETTING)0));

	TEST_ASSERT_FALSE(Settings_stringIsSet((STRINGSETTING)0));
	Settings_setString((STRINGSETTING)0, "TESTSETTING");
	TEST_ASSERT_TRUE(Settings_stringIsSet((STRINGSETTING)0));
}

static void test_GetString_ReturnsCorrectStringWhenSet(void)
{
	char expected[] = "TESTSETTING";
	Settings_setString((STRINGSETTING)0, expected);
	char * returned = Settings_getString((STRINGSETTING)0);
	TEST_ASSERT_EQUAL_STRING(expected, returned);
}

static void test_GetInt_ReturnsCorrectIntWhenSet(void)
{
	Settings_setInt((INTSETTING)0, 10);
	TEST_ASSERT_EQUAL(10, Settings_getInt((INTSETTING)0));
}

static void test_SettingsCanOnlyBeSetOnce(void)
{
	Settings_setString((STRINGSETTING)0, "TESTSETTING");
	
	char newStringSetting[] = "THIS SHOULDN'T WORK";
	char expected[] = "TESTSETTING";
	Settings_setString((STRINGSETTING)0, newStringSetting);
	char * returned = Settings_getString((STRINGSETTING)0);
	TEST_ASSERT_EQUAL_STRING(expected, returned);

	Settings_setInt((INTSETTING)0, 100);
	Settings_setInt((INTSETTING)0, 10000);
	TEST_ASSERT_EQUAL(100, Settings_getInt((INTSETTING)0));
}

static void test_AttemptToAccessStringSettingOutsideRange_ReturnsNULL(void)
{
	int invalid_setting = STRING_SETTINGS_COUNT;
	char * result = Settings_getString((STRINGSETTING)invalid_setting);
	TEST_ASSERT_NULL(result);
}

static void test_AttemptToAccessIntSettingOutsideRange_ReturnsZero(void)
{
	int invalid_setting = INT_SETTINGS_COUNT;
	TEST_ASSERT_EQUAL(0, Settings_getInt((INTSETTING)invalid_setting));
}

static void test_GetStringSettingName_ReturnsCorrectName(void)
{
	TEST_ASSERT_EQUAL_STRING("GPRS_APN", Settings_getStringName(GPRS_APN));
}

static void test_GetIntSettingName_ReturnsCorrectName(void)
{
	TEST_ASSERT_EQUAL_STRING("THINGSPEAK_UPLOAD_INTERVAL_SECS", Settings_getIntName(THINGSPEAK_UPLOAD_INTERVAL_SECS));
}

int main(void)
{
	UnityBegin("DLSettings.cpp");

  	RUN_TEST(test_GetString_ReturnsNULLIfNotSet);
  	RUN_TEST(test_GetInt_ReturnsZeroIfNotSet);
  	RUN_TEST(test_isSetReturnsCorrectValue);
  	RUN_TEST(test_GetString_ReturnsCorrectStringWhenSet);
  	RUN_TEST(test_GetInt_ReturnsCorrectIntWhenSet);
  	RUN_TEST(test_SettingsCanOnlyBeSetOnce);
  	RUN_TEST(test_AttemptToAccessStringSettingOutsideRange_ReturnsNULL);
	RUN_TEST(test_AttemptToAccessIntSettingOutsideRange_ReturnsZero);
	RUN_TEST(test_GetStringSettingName_ReturnsCorrectName);
	RUN_TEST(test_GetIntSettingName_ReturnsCorrectName);
	
  	UnityEnd();
  	return 0;
}

