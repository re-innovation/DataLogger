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

/*
 * Local Application Includes
 */

#include "../DLSettings.h"

/*
 * Unity Test Framework
 */

#include "unity.h"

static void test_GetString_ReturnsNULLIfNotSet(void)
{
	TEST_ASSERT_NULL(Settings_getString((STRINGSETTING)0));
}

static void test_GetInt_ReturnsZeroIfNotSet(void)
{
	TEST_ASSERT_EQUAL(0, Settings_getInt((INTSETTING)0));
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
	char newStringSetting[] = "THIS SHOULDN'T WORK";
	char expected[] = "TESTSETTING";
	Settings_setString((STRINGSETTING)0, newStringSetting);
	char * returned = Settings_getString((STRINGSETTING)0);
	TEST_ASSERT_EQUAL_STRING(expected, returned);

	Settings_setInt((INTSETTING)0, 10000);
	TEST_ASSERT_EQUAL(10, Settings_getInt((INTSETTING)0));
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
	TEST_ASSERT_EQUAL_STRING("CSV_RECORD_INTERVAL", Settings_getIntName(CSV_RECORD_INTERVAL));
}

int main(void)
{
	UnityBegin("DLSettings.cpp");

  	RUN_TEST(test_GetString_ReturnsNULLIfNotSet);
  	RUN_TEST(test_GetInt_ReturnsZeroIfNotSet);
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

