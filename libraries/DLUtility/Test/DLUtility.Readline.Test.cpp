/*
 * DLUtility.Readline.Test.cpp
 * 
 * Tests the Readline functionality of the utility module
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

#include "..\DLutility.h"

/*
 * Unity Test Framework
 */

#include "unity.h"

static char * pNext;
static char readBuffer[300];

static char readFunction(void)
{
	return *pNext++;
}

void setUp(void)
{
	memset(readBuffer, 'A', 300);
}

static void test_ReadlineWithBlankInput_ReturnsZeroReadChars(void)
{
	char emptyString[] = "";
	pNext = emptyString;
	uint32_t n = readLineWithReadFunction(readFunction, readBuffer, 300);
	TEST_ASSERT_EQUAL_STRING("", readBuffer);
	TEST_ASSERT_EQUAL(0, n);
}

static void test_ReadlineWithLF_ReturnsLF(void)
{
	char LF[] = "\nNext_Line_Should_Not_Appear";
	pNext = LF;
	uint32_t n = readLineWithReadFunction(readFunction, readBuffer, 300);
	TEST_ASSERT_EQUAL_STRING("\n", readBuffer);
	TEST_ASSERT_EQUAL(1, n);
}

static void test_ReadlineWithCRLF_ReturnsCRLF(void)
{
	char CRLF[] = "\r\nNext_Line_Should_Not_Appear";
	pNext = CRLF;
	uint32_t n = readLineWithReadFunction(readFunction, readBuffer, 300);
	TEST_ASSERT_EQUAL_STRING("\r\n", readBuffer);
	TEST_ASSERT_EQUAL(2, n);
}

static void test_ReadlineWithNormalStringAndLF_ReturnsThatString(void)
{
	char string[] = "String!\nNext_Line_Should_Not_Appear";
	pNext = string;
	uint32_t n = readLineWithReadFunction(readFunction, readBuffer, 300);
	TEST_ASSERT_EQUAL_STRING("String!\n", readBuffer);
	TEST_ASSERT_EQUAL(8, n);
}

static void test_ReadlineWithNormalStringAndCRLF_ReturnsThatString(void)
{
	char string[] = "String!\r\nNext_Line_Should_Not_Appear";
	pNext = string;
	uint32_t n = readLineWithReadFunction(readFunction, readBuffer, 300);
	TEST_ASSERT_EQUAL_STRING("String!\r\n", readBuffer);
	TEST_ASSERT_EQUAL(9, n);
}

static void test_ReadlineWithLimitOfNChars_ShouldReturnAtMostNChars(void)
{
	char string[] = "LongString...\r\nNext_Line_Should_Not_Appear";
	pNext = string;
	uint32_t n = readLineWithReadFunction(readFunction, readBuffer, 5);
	// No NULL terminator, so test each char individually
	TEST_ASSERT_EQUAL('L', readBuffer[0]);
	TEST_ASSERT_EQUAL('o', readBuffer[1]);
	TEST_ASSERT_EQUAL('n', readBuffer[2]);
	TEST_ASSERT_EQUAL('g', readBuffer[3]);
	TEST_ASSERT_EQUAL('S', readBuffer[4]);
	TEST_ASSERT_EQUAL('A', readBuffer[5]);
	TEST_ASSERT_EQUAL(5, n);
}


static void test_ReadlineWithStripChars_ShouldStripLF(void)
{
	char string[] = "String!\nNext_Line_Should_Not_Appear";
	pNext = string;
	uint32_t n = readLineWithReadFunction(readFunction, readBuffer, 300, true);
	TEST_ASSERT_EQUAL_STRING("String!", readBuffer);
	TEST_ASSERT_EQUAL(7, n);
}

static void test_ReadlineWithStripChars_ShouldStripCRLF(void)
{
	char string[] = "String!\r\nNext_Line_Should_Not_Appear";
	pNext = string;
	uint32_t n = readLineWithReadFunction(readFunction, readBuffer, 300, true);
	TEST_ASSERT_EQUAL_STRING("String!", readBuffer);
	TEST_ASSERT_EQUAL(7, n);
}

int main(void)
{
  UnityBegin("DLDataField.cpp");

  RUN_TEST(test_ReadlineWithBlankInput_ReturnsZeroReadChars);
  RUN_TEST(test_ReadlineWithLF_ReturnsLF);
  RUN_TEST(test_ReadlineWithCRLF_ReturnsCRLF);
  RUN_TEST(test_ReadlineWithNormalStringAndLF_ReturnsThatString);
  RUN_TEST(test_ReadlineWithNormalStringAndCRLF_ReturnsThatString);
  RUN_TEST(test_ReadlineWithLimitOfNChars_ShouldReturnAtMostNChars);
  RUN_TEST(test_ReadlineWithStripChars_ShouldStripLF);
  RUN_TEST(test_ReadlineWithStripChars_ShouldStripCRLF);

  UnityEnd();
  return 0;
}
