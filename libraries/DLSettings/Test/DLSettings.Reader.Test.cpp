/*
 * DLSettings.Reader.Test.cpp
 *
 * Tests the Settings reader class
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
#include "../DLSettings.Reader.h"

/*
 * Unity Test Framework
 */

#include "unity.h"

void setUp(void)
{
  Settings_Reset();
}

void test_ReadingFromNULLStringReturnsFalse(void)
{
	TEST_ASSERT_FALSE(Settings_ReadFromString(NULL));
}

void test_ReadingFromInvalidStringReturnsFalse(void)
{
	TEST_ASSERT_FALSE(Settings_ReadFromString("SOME INVALID STRING"));
}

void test_ReadingFromUnrecognizedSettingReturnsFalse(void)
{
  TEST_ASSERT_FALSE(Settings_ReadFromString("NOT_AN_SETTING_NAME=NOTANINTEGER"));
}

void test_ReadingFromInvalidIntSettingReturnsFalse(void)
{
	TEST_ASSERT_FALSE(Settings_ReadFromString("THINGSPEAK_UPLOAD_INTERVAL=NOTANINTEGER"));
}

void test_ReadingFromValidIntSettingReturnsTrueAndSetsThatSetting(void)
{
  TEST_ASSERT_TRUE(Settings_ReadFromString("THINGSPEAK_UPLOAD_INTERVAL=30"));
  TEST_ASSERT_EQUAL(30, Settings_getInt(THINGSPEAK_UPLOAD_INTERVAL));
}

void test_ReadingFromValidStringSettingReturnsTrueAndSetsThatSetting(void)
{
  TEST_ASSERT_TRUE(Settings_ReadFromString("GPRS_APN=www.exampleapn.com"));
  TEST_ASSERT_EQUAL_STRING("www.exampleapn.com", Settings_getString(GPRS_APN));
  TEST_ASSERT_TRUE(Settings_ReadFromString("GPRS_APN=www.exampleapn.com"));
  TEST_ASSERT_EQUAL_STRING("www.exampleapn.com", Settings_getString(GPRS_APN));
}

int main(void)
{
    UnityBegin("DLSettings.Reader.cpp");

  	RUN_TEST(test_ReadingFromNULLStringReturnsFalse);
  	RUN_TEST(test_ReadingFromInvalidStringReturnsFalse);
    RUN_TEST(test_ReadingFromUnrecognizedSettingReturnsFalse);
  	RUN_TEST(test_ReadingFromInvalidIntSettingReturnsFalse);
    RUN_TEST(test_ReadingFromValidIntSettingReturnsTrueAndSetsThatSetting);
    RUN_TEST(test_ReadingFromValidStringSettingReturnsTrueAndSetsThatSetting);

  	UnityEnd();
  	return 0;
}

