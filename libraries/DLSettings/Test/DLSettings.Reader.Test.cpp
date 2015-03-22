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

void test_ReadingFromNULLStringReturnsFalse(void)
{
	TEST_ASSERT_FALSE(Settings_ReadFromString(NULL));
}

void test_ReadingFromInvalidStringReturnsFalse(void)
{
	TEST_ASSERT_FALSE(Settings_ReadFromString("SOME INVALID STRING"));
}

void test_ReadingFromUnrecognizedStringReturnsFalse(void)
{
	TEST_ASSERT_FALSE(Settings_ReadFromString("SOME INVALID STRING"));
}

int main(void)
{
	UnityBegin("DLSettings.Reader.cpp");

  	RUN_TEST(test_ReadingFromNULLStringReturnsFalse);
  	RUN_TEST(test_ReadingFromInvalidStringReturnsFalse);
  	RUN_TEST(test_ReadingFromUnrecognizedStringReturnsFalse);
  	
  	UnityEnd();
  	return 0;
}

