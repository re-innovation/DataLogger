/*
 * DLCSV.Test.cpp
 * 
 * Tests the CSV utility functionality
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
#include <iostream>
#include <string>
#include <vector>

/*
 * Local Application Includes
 */

#include "DLUtility.h"
#include "DLCSV.h"

/*
 * Unity Test Framework
 */

#include "unity.h"

void test_TimestampsAreCreatedSuccessfully(void)
{
	TM testTime;

	testTime.tm_year = 115; // 2015 as years since 1900
	testTime.tm_mon = 4;
	testTime.tm_mday = 3;
	testTime.tm_hour = 13;
	testTime.tm_min = 9;
	testTime.tm_sec = 34;

	char buffer[100];

	CSV_writeTimestampToBuffer(&testTime, buffer);

	TEST_ASSERT_EQUAL_STRING("2015-04-03 13:09:34 +0000", buffer);
}

int main(void)
{
    UnityBegin("DLCSV.Test.cpp");

 	RUN_TEST(test_TimestampsAreCreatedSuccessfully);

    return 0;
}
