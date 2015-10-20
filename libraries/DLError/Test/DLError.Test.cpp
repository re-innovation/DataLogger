/*
 * DLError.Test.cpp
 *
 * Tests the Error module
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

#include <iostream>

/*
 * DataLogger Includes
 */

#include "DLError.h"

/*
 * Unity Test Framework
 */

#include "unity.h"

static char s_last_error_str[256];
static FATAL_ERROR_ENUM s_last_fatal_error;
static RUNNING_ERROR_ENUM s_last_running_error;

static void fatal_error_handler(char const * const err, FATAL_ERROR_ENUM errorType)
{
	strcpy(s_last_error_str, err);
	s_last_fatal_error = errorType;
}

static void running_error_handler(RUNNING_ERROR_ENUM errorType)
{
	s_last_running_error = errorType;
}

void setUp(void)
{
	Error_Setup(fatal_error_handler, running_error_handler);
}

static void test_FatalErrorsAreReportedToApplication(void)
{
	Error_Fatal("Fatal config error!", ERR_FATAL_CONFIG);
	TEST_ASSERT_EQUAL_STRING("Fatal config error!", s_last_error_str);
	TEST_ASSERT_EQUAL(ERR_FATAL_CONFIG, s_last_fatal_error);
	TEST_ASSERT_EQUAL(ERR_FATAL_CONFIG, Error_Get_Fatal_Error());

	Error_Fatal("Fatal channel error!", ERR_FATAL_CHANNEL);
	TEST_ASSERT_EQUAL_STRING("Fatal channel error!", s_last_error_str);
	TEST_ASSERT_EQUAL(ERR_FATAL_CHANNEL, s_last_fatal_error);
	TEST_ASSERT_EQUAL(ERR_FATAL_CHANNEL, Error_Get_Fatal_Error());
}

static void test_AllErrorsAreClearedAtSetup(void)
{
	TEST_ASSERT_EQUAL(ERR_RUNNING_NONE, Error_Get_Running_Error());
	TEST_ASSERT_EQUAL(ERR_FATAL_NONE, Error_Get_Fatal_Error());
}

static void test_SingleRunningErrorIsReportedCorrectly(void)
{
	Error_Running(ERR_RUNNING_BATT_LEVEL_LOW, true);
	TEST_ASSERT_EQUAL(ERR_RUNNING_BATT_LEVEL_LOW, Error_Get_Running_Error());
	TEST_ASSERT_EQUAL(ERR_RUNNING_BATT_LEVEL_LOW, s_last_running_error);
	Error_Running(ERR_RUNNING_BATT_LEVEL_LOW, false);

	Error_Running(ERR_RUNNING_DATA_UPLOAD_FAILED, true);
	TEST_ASSERT_EQUAL(ERR_RUNNING_DATA_UPLOAD_FAILED, Error_Get_Running_Error());
	TEST_ASSERT_EQUAL(ERR_RUNNING_DATA_UPLOAD_FAILED, s_last_running_error);
	Error_Running(ERR_RUNNING_DATA_UPLOAD_FAILED, false);

	Error_Running(ERR_RUNNING_NO_GPS, true);
	TEST_ASSERT_EQUAL(ERR_RUNNING_NO_GPS, Error_Get_Running_Error());
	TEST_ASSERT_EQUAL(ERR_RUNNING_NO_GPS, s_last_running_error);
	Error_Running(ERR_RUNNING_NO_GPS, false);

	TEST_ASSERT_EQUAL(ERR_RUNNING_NONE, Error_Get_Running_Error());
	TEST_ASSERT_EQUAL(ERR_RUNNING_NONE, s_last_running_error);
}

static void test_ErrorPrioritiesAreObservedCorrectly(void)
{
	Error_Running(ERR_RUNNING_NO_GPS, true);
	TEST_ASSERT_EQUAL(ERR_RUNNING_NO_GPS, Error_Get_Running_Error());
	TEST_ASSERT_EQUAL(ERR_RUNNING_NO_GPS, s_last_running_error);

	Error_Running(ERR_RUNNING_DATA_UPLOAD_FAILED, true);
	TEST_ASSERT_EQUAL(ERR_RUNNING_DATA_UPLOAD_FAILED, Error_Get_Running_Error());
	TEST_ASSERT_EQUAL(ERR_RUNNING_DATA_UPLOAD_FAILED, s_last_running_error);

	Error_Running(ERR_RUNNING_BATT_LEVEL_LOW, true);
	TEST_ASSERT_EQUAL(ERR_RUNNING_BATT_LEVEL_LOW, Error_Get_Running_Error());
	TEST_ASSERT_EQUAL(ERR_RUNNING_BATT_LEVEL_LOW, s_last_running_error);

	Error_Running(ERR_RUNNING_BATT_LEVEL_LOW, false);
	TEST_ASSERT_EQUAL(ERR_RUNNING_DATA_UPLOAD_FAILED, Error_Get_Running_Error());
	TEST_ASSERT_EQUAL(ERR_RUNNING_DATA_UPLOAD_FAILED, s_last_running_error);

	Error_Running(ERR_RUNNING_DATA_UPLOAD_FAILED, false);
	TEST_ASSERT_EQUAL(ERR_RUNNING_NO_GPS, Error_Get_Running_Error());
	TEST_ASSERT_EQUAL(ERR_RUNNING_NO_GPS, s_last_running_error);

	Error_Running(ERR_RUNNING_NO_GPS, false);
	TEST_ASSERT_EQUAL(ERR_RUNNING_NONE, Error_Get_Running_Error());
	TEST_ASSERT_EQUAL(ERR_RUNNING_NONE, s_last_running_error);
}

int main(void)
{
    UnityBegin("DLError.cpp");

    RUN_TEST(test_FatalErrorsAreReportedToApplication);
    RUN_TEST(test_AllErrorsAreClearedAtSetup);
    RUN_TEST(test_SingleRunningErrorIsReportedCorrectly);
    RUN_TEST(test_ErrorPrioritiesAreObservedCorrectly);

    UnityEnd();
    return 0;
}