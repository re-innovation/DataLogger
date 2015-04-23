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

void test_ReadingFromNULLStringReturnsCorrectError(void)
{
	TEST_ASSERT_EQUAL(ERR_READER_NO_STRING, Settings_readFromString(NULL));
  TEST_ASSERT_EQUAL(ERR_READER_NO_STRING, Settings_getLastReaderResult());
  TEST_ASSERT_EQUAL_STRING(ERROR_STR_NO_STRING, Settings_getLastReaderResultText());
}

void test_ReadingFromInvalidStringReturnsCorrectError(void)
{
	TEST_ASSERT_EQUAL(ERR_READER_NO_EQUALS, Settings_readFromString("SOME INVALID STRING"));
  TEST_ASSERT_EQUAL(ERR_READER_NO_EQUALS, Settings_getLastReaderResult());
  TEST_ASSERT_EQUAL_STRING(ERROR_STR_NO_EQUALS, Settings_getLastReaderResultText());
}

void test_ReadingFromUnrecognizedSettingReturnsCorrectError(void)
{
  TEST_ASSERT_EQUAL(ERR_READER_NO_NAME, Settings_readFromString("NOT_AN_SETTING_NAME=NOTANINTEGER"));
  TEST_ASSERT_EQUAL(ERR_READER_NO_NAME, Settings_getLastReaderResult());

  char expected[100];
  sprintf(expected, ERROR_STR_NO_NAME, "NOT_AN_SETTING_NAME");
  TEST_ASSERT_EQUAL_STRING(expected, Settings_getLastReaderResultText());
}

void test_ReadingFromInvalidIntSettingReturnsCorrectError(void)
{
	TEST_ASSERT_EQUAL(ERR_READER_INVALID_INT, Settings_readFromString("THINGSPEAK_UPLOAD_INTERVAL_SECS=NOTANINTEGER"));
  TEST_ASSERT_EQUAL(ERR_READER_INVALID_INT, Settings_getLastReaderResult());

  char expected[100];
  sprintf(expected, ERROR_STR_INVALID_INT, "NOTANINTEGER", "THINGSPEAK_UPLOAD_INTERVAL_SECS");
  TEST_ASSERT_EQUAL_STRING(expected, Settings_getLastReaderResultText());
}

void test_ReadingFromValidIntSettingReturnsNoErrorAndSetsThatSetting(void)
{
  TEST_ASSERT_EQUAL(ERR_READER_NONE, Settings_readFromString("THINGSPEAK_UPLOAD_INTERVAL_SECS=30"));
  TEST_ASSERT_EQUAL(30, Settings_getInt(THINGSPEAK_UPLOAD_INTERVAL_SECS));
}

void test_ReadingFromValidStringSettingReturnsNoErrorAndSetsThatSetting(void)
{
  TEST_ASSERT_EQUAL(ERR_READER_NONE, Settings_readFromString("GPRS_APN=www.exampleapn.com"));
  TEST_ASSERT_EQUAL_STRING("www.exampleapn.com", Settings_getString(GPRS_APN));
}

void test_ReadingFromValidStringSettingWithSpacesReturnsNoErrorAndSetsThatSetting(void)
{
  TEST_ASSERT_EQUAL(ERR_READER_NONE, Settings_readFromString("GPRS_APN = www.exampleapn.com"));
  TEST_ASSERT_EQUAL_STRING("www.exampleapn.com", Settings_getString(GPRS_APN));
}

void test_CommentLinesAreIgnoredButValid(void)
{
  TEST_ASSERT_EQUAL(ERR_READER_NONE, Settings_readFromString("# This is a comment"));
}

void test_AllRequiredSettingsMustBeParsedBeforeReaderValidates(void)
{
  Settings_requireInt(THINGSPEAK_UPLOAD_INTERVAL_SECS);
  Settings_requireString(GPRS_APN);
  TEST_ASSERT_FALSE(Settings_allRequiredSettingsRead());

  Settings_readFromString("GPRS_APN = www.exampleapn.com");
  TEST_ASSERT_FALSE(Settings_allRequiredSettingsRead());

  Settings_readFromString("THINGSPEAK_UPLOAD_INTERVAL_SECS=30");
  TEST_ASSERT_TRUE(Settings_allRequiredSettingsRead());
}

int main(void)
{
    UnityBegin("DLSettings.Reader.cpp");

  	RUN_TEST(test_ReadingFromNULLStringReturnsCorrectError);
  	RUN_TEST(test_ReadingFromInvalidStringReturnsCorrectError);
    RUN_TEST(test_ReadingFromUnrecognizedSettingReturnsCorrectError);
  	RUN_TEST(test_ReadingFromInvalidIntSettingReturnsCorrectError);
    RUN_TEST(test_ReadingFromValidIntSettingReturnsNoErrorAndSetsThatSetting);
    RUN_TEST(test_ReadingFromValidStringSettingReturnsNoErrorAndSetsThatSetting);
    RUN_TEST(test_ReadingFromValidStringSettingWithSpacesReturnsNoErrorAndSetsThatSetting);
    RUN_TEST(test_CommentLinesAreIgnoredButValid);
    RUN_TEST(test_AllRequiredSettingsMustBeParsedBeforeReaderValidates);
    
  	UnityEnd();
  	return 0;
}

