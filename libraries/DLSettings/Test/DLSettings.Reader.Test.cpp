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

#include "DLLocalStorage.h"
#include "DLSettings.h"
#include "DLSettings.Global.h"
#include "DLSettings.Reader.h"
#include "DLSettings.Reader.Errors.h"
#include "DLSettings.Global.Reader.h"

/*
 * Unity Test Framework
 */

#include "unity.h"

void setUp(void)
{
  Settings_InitGlobal();
  Settings_InitReader();
}

void test_ReadingFromNULLStringReturnsCorrectError(void)
{
	TEST_ASSERT_EQUAL(ERR_READER_NO_STRING, Settings_readFromString(NULL, 1));
  TEST_ASSERT_EQUAL(ERR_READER_NO_STRING, Settings_getLastReaderResult());
  TEST_ASSERT_EQUAL_STRING(ERROR_STR_NO_STRING, Settings_getLastReaderResultText());
}

void test_ReadingFromInvalidStringReturnsCorrectError(void)
{
	TEST_ASSERT_EQUAL(ERR_READER_NO_EQUALS, Settings_readFromString("SOME INVALID STRING", 2));
  TEST_ASSERT_EQUAL(ERR_READER_NO_EQUALS, Settings_getLastReaderResult());

  char expected[100];
  sprintf(expected, ERROR_STR_NO_EQUALS, 2);
  TEST_ASSERT_EQUAL_STRING(expected, Settings_getLastReaderResultText());
}

void test_ReadingFromUnrecognizedSettingReturnsCorrectError(void)
{
  TEST_ASSERT_EQUAL(ERR_READER_NO_NAME, Settings_readFromString("NOT_AN_SETTING_NAME=NOTANINTEGER", 3));
  TEST_ASSERT_EQUAL(ERR_READER_NO_NAME, Settings_getLastReaderResult());

  char expected[100];
  sprintf(expected, ERROR_STR_NO_NAME, 3, "NOT_AN_SETTING_NAME");
  TEST_ASSERT_EQUAL_STRING(expected, Settings_getLastReaderResultText());
}

void test_ReadingFromInvalidIntSettingReturnsCorrectError(void)
{
	TEST_ASSERT_EQUAL(ERR_READER_INVALID_INT, Settings_readFromString("DATA_UPLOAD_INTERVAL_SECS=NOTANINTEGER", 4));
  TEST_ASSERT_EQUAL(ERR_READER_INVALID_INT, Settings_getLastReaderResult());

  char expected[100];
  sprintf(expected, ERROR_STR_INVALID_INT, 4, "NOTANINTEGER", "DATA_UPLOAD_INTERVAL_SECS");
  TEST_ASSERT_EQUAL_STRING(expected, Settings_getLastReaderResultText());
}

void test_ReadingFromValidIntSettingReturnsNoErrorAndSetsThatSetting(void)
{
  TEST_ASSERT_EQUAL(ERR_READER_NONE, Settings_readFromString("DATA_UPLOAD_INTERVAL_SECS=30", 6));
  TEST_ASSERT_EQUAL(30, Settings_getInt(DATA_UPLOAD_INTERVAL_SECS));
}

void test_ReadingFromValidStringSettingReturnsNoErrorAndSetsThatSetting(void)
{
  TEST_ASSERT_EQUAL(ERR_READER_NONE, Settings_readFromString("GPRS_APN=www.exampleapn.com", 7));
  TEST_ASSERT_EQUAL_STRING("www.exampleapn.com", Settings_getString(GPRS_APN));
}

void test_ReadingFromValidStringSettingWithSpacesReturnsNoErrorAndSetsThatSetting(void)
{
  TEST_ASSERT_EQUAL(ERR_READER_NONE, Settings_readFromString("GPRS_APN = www.exampleapn.com", 8));
  TEST_ASSERT_EQUAL_STRING("www.exampleapn.com", Settings_getString(GPRS_APN));
}

void test_CommentLinesAreIgnoredButValid(void)
{
  TEST_ASSERT_EQUAL(ERR_READER_NONE, Settings_readFromString("# This is a comment", 9));
}

void test_BlankLinesAreIgnoredButValid(void)
{
  TEST_ASSERT_EQUAL(ERR_READER_NONE, Settings_readFromString("", 10));
  TEST_ASSERT_EQUAL(ERR_READER_NONE, Settings_readFromString(" ", 11));
  TEST_ASSERT_EQUAL(ERR_READER_NONE, Settings_readFromString("\t", 12));
}

void test_AllRequiredSettingsMustBeParsedBeforeReaderValidates(void)
{
  Settings_requireInt(DATA_UPLOAD_INTERVAL_SECS);
  Settings_requireString(GPRS_APN);
  TEST_ASSERT_FALSE(Settings_allRequiredSettingsRead());

  Settings_readFromString("GPRS_APN = www.exampleapn.com", 13);
  TEST_ASSERT_FALSE(Settings_allRequiredSettingsRead());

  Settings_readFromString("DATA_UPLOAD_INTERVAL_SECS=30", 14);
  TEST_ASSERT_TRUE(Settings_allRequiredSettingsRead());
}

void test_SettingCountIsCorrect(void)
{
    TEST_ASSERT_EQUAL(0, Settings_getCount());
    TEST_ASSERT_EQUAL(0, Settings_getIntCount());
    TEST_ASSERT_EQUAL(0, Settings_getStringCount());
    Settings_readFromString("GPRS_APN = www.exampleapn.com", 15);
    TEST_ASSERT_EQUAL(1, Settings_getCount());
    TEST_ASSERT_EQUAL(0, Settings_getIntCount());
    TEST_ASSERT_EQUAL(1, Settings_getStringCount());
 
    Settings_readFromString("DATA_UPLOAD_INTERVAL_SECS=30", 16);
    TEST_ASSERT_EQUAL(2, Settings_getCount());
    TEST_ASSERT_EQUAL(1, Settings_getIntCount());
    TEST_ASSERT_EQUAL(1, Settings_getStringCount());   
}

void test_readFromFileReturnsNoFileErrorForMissingFile(void)
{
    LocalStorageInterface * s_storage = LocalStorage_GetLocalStorageInterface((LOCAL_STORAGE_TYPE)0);
    TEST_ASSERT_EQUAL(ERR_READER_NO_FILE, Settings_readGlobalFromFile(s_storage, "Not a file"));
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
    RUN_TEST(test_BlankLinesAreIgnoredButValid);

    RUN_TEST(test_AllRequiredSettingsMustBeParsedBeforeReaderValidates);
    RUN_TEST(test_SettingCountIsCorrect);

    RUN_TEST(test_readFromFileReturnsNoFileErrorForMissingFile);

  	UnityEnd();
  	return 0;
}

