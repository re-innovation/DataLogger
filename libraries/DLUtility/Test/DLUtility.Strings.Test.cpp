#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "unity.h"

#include "../DLUtility.Strings.h"

static char buffer[100];
static FixedLengthAccumulator * accumulator;

void test_toLowerStr_DoesNothingToEmptyString(void)
{
    buffer[0] = '\0';
    toLowerStr(buffer);
    TEST_ASSERT_EQUAL(0, strlen(buffer));
}

void test_toLowerStr_DoesNothingToLowerCaseString(void)
{
    strcpy(buffer, "lowercase string");
    toLowerStr(buffer);
    TEST_ASSERT_EQUAL_STRING("lowercase string", buffer);
}

void test_toLowerStr_CorrectlyLowercasesMixedCaseString(void)
{
    strcpy(buffer, "MixED CaSe sTrIng");
    toLowerStr(buffer);
    TEST_ASSERT_EQUAL_STRING("mixed case string", buffer);
}

void test_strncpy_safe_CorrectlyCopiesStringLessThanMaxLength(void)
{
    char destination[10];
    char source[] = "ABC";

    strncpy_safe(destination, source, sizeof(destination));

    TEST_ASSERT_EQUAL_STRING("ABC", destination);
}

void test_strncpy_safe_CorrectlyCopiesStringEqualToThanMaxLength(void)
{
    char destination[10];
    char source[] = "ABCDEFGHI";

    strncpy_safe(destination, source, sizeof(destination));

    TEST_ASSERT_EQUAL_STRING("ABCDEFGHI", destination);
}

void test_strncpy_safe_CorrectlyCopiesStringGreaterThanMaxLength(void)
{
    char destination[10];
    char source[] = "ABCDEFGHIJKLMNO";

    strncpy_safe(destination, source, sizeof(destination));

    TEST_ASSERT_EQUAL_STRING("ABCDEFGHI", destination);
}

void test_strncpy_safe_CorrectlyCopiesStringOfRequestedLength(void)
{
    char destination[10];
    char source[] = "ABCDEFGHIJKLMNO";

    strncpy_safe(destination, source, 4);

    TEST_ASSERT_EQUAL_STRING("ABC", destination);
}

void test_strncpy_safe_CorrectlyCopiesAllOfSourseWhenRequestingMoreThanLength(void)
{
    char destination[20];
    char source[] = "ABCDEFGHIJKLMNO";

    strncpy_safe(destination, source, 100);

    TEST_ASSERT_EQUAL_STRING("ABCDEFGHIJKLMNO", destination);
}

void test_skipSpacesCorrectlyJumpsOverWhitespace(void)
{
    char test[] = "Nowhitespaceinstring";
    char * out = skipSpaces(test);
    TEST_ASSERT_EQUAL_PTR(test, out);

    char test2[] = "     spacesAtStart";
    out = skipSpaces(test2);
    TEST_ASSERT_EQUAL_STRING("spacesAtStart", out);
    
    char test3[] = "";
    out = skipSpaces(test3);
    TEST_ASSERT_EQUAL_PTR(test3, out);

    char test4[] = "\t";
    out = skipSpaces(test4);
    TEST_ASSERT_EQUAL_PTR(test4+1, out);

    char test5[] = " ";
    out = skipSpaces(test5);
    TEST_ASSERT_EQUAL_PTR(test5+1, out);
}

void test_stringIsWhitespaceReturnsTrueForWhitespaceOrBlankAndTrueOtherwise(void)
{
    TEST_ASSERT_TRUE(stringIsWhitespace(""));
    TEST_ASSERT_TRUE(stringIsWhitespace(" "));
    TEST_ASSERT_TRUE(stringIsWhitespace("\t"));
    TEST_ASSERT_FALSE(stringIsWhitespace(NULL));    
    TEST_ASSERT_FALSE(stringIsWhitespace("1"));
    TEST_ASSERT_FALSE(stringIsWhitespace(" 1"));
    TEST_ASSERT_FALSE(stringIsWhitespace("1 "));
}

void test_FixedLengthAccumulator_InitsCorrectly(void)
{
    accumulator = new FixedLengthAccumulator(buffer, 100);

    TEST_ASSERT_EQUAL_PTR(buffer, accumulator->c_str());
    TEST_ASSERT_EQUAL(0, strlen(accumulator->c_str()));
}

void test_FixedLengthAccumulator_FillsUpToMaxLength(void)
{
    uint8_t i;
    
    //Fill up to 98th char (leave one free)
    for (i = 0; i < 98; i++)
    {
        TEST_ASSERT_TRUE( accumulator->writeChar('A') );
        TEST_ASSERT_EQUAL(i+1, strlen(accumulator->c_str()));
        TEST_ASSERT_FALSE( accumulator->isFull() );
    }
    
    // When the last char is filled, accumulator should be full
    TEST_ASSERT_TRUE( accumulator->writeChar('A') );
    TEST_ASSERT_EQUAL(99, strlen(accumulator->c_str()));
    TEST_ASSERT_TRUE( accumulator->isFull() );
    
    // Further attempts to write should return false
    TEST_ASSERT_FALSE( accumulator->writeChar('A') );
    TEST_ASSERT_EQUAL(99, strlen(accumulator->c_str()));
    TEST_ASSERT_TRUE( accumulator->isFull() );
    
}

void test_FixedLengthAccumulator_ResetsCorrectly(void)
{
    accumulator->reset();
    
    TEST_ASSERT_EQUAL_PTR(buffer, accumulator->c_str());
    TEST_ASSERT_EQUAL(0, strlen(accumulator->c_str()));
}

void test_FixedLengthAccumulator_AcceptStringLessThanMaxLength(void)
{
    TEST_ASSERT_TRUE(accumulator->writeString(
        "ThisStringIs98CharsLongAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"));
        
    TEST_ASSERT_EQUAL(98, strlen(accumulator->c_str()));
}

void test_FixedLengthAccumulator_CopiesPartialStringUpToMaxLength(void)  
{
    TEST_ASSERT_FALSE(accumulator->writeString("AAA"));
    TEST_ASSERT_EQUAL(99, strlen(accumulator->c_str()));
}

void test_FixedLengthAccumulator_WritesCRLFUsingWriteLine(void)
{
    accumulator->reset();
    accumulator->writeLine("Should have CRLF at end");
    TEST_ASSERT_EQUAL_STRING("Should have CRLF at end\r\n", buffer);
}

void test_FixedLengthAccumulator_CorrectlyReturnsCurrentLength(void)
{
    accumulator->reset();
    TEST_ASSERT_EQUAL(0, accumulator->length());
    accumulator->writeString("Thislineis26characterslong");
    TEST_ASSERT_EQUAL(26, accumulator->length());
    accumulator->writeChar('a');
    TEST_ASSERT_EQUAL(27, accumulator->length());
    accumulator->writeString("MAX OUT THE ACCUMULATOR - MAX OUT THE ACCUMULATOR - MAX OUT THE ACCUMULATOR");
    TEST_ASSERT_EQUAL(99, accumulator->length());
}

void test_FixedLengthAccumulator_CorrectlyRemovesAndAddsNewContent(void)
{
    accumulator->reset();
    accumulator->writeString("This is some content, ");
    accumulator->remove(2);

    TEST_ASSERT_EQUAL(20, accumulator->length());
    TEST_ASSERT_EQUAL_STRING("This is some content", buffer);
    accumulator->writeString(", this is some more content");

    TEST_ASSERT_EQUAL(47, accumulator->length());
    TEST_ASSERT_EQUAL_STRING("This is some content, this is some more content", buffer);
}

void test_SplitAndStripWhitespaceErrorsWithInvalidStrings(void)
{
    char * pLStart;
    char * pLEnd;
    char * pRStart;
    char * pREnd;

    TEST_ASSERT_FALSE(splitAndStripWhiteSpace(NULL, '=', &pLStart, &pLEnd, &pRStart, &pREnd));
    TEST_ASSERT_FALSE(splitAndStripWhiteSpace((char*)"", '=', &pLStart, &pLEnd, &pRStart, &pREnd));
    TEST_ASSERT_FALSE(splitAndStripWhiteSpace((char*)"NoEqualsSign", '=', &pLStart, &pLEnd, &pRStart, &pREnd));
    TEST_ASSERT_FALSE(splitAndStripWhiteSpace((char*)"NoRightHandSide=", '=', &pLStart, &pLEnd, &pRStart, &pREnd));
    TEST_ASSERT_FALSE(splitAndStripWhiteSpace((char*)"=NoLeftHandSide", '=', &pLStart, &pLEnd, &pRStart, &pREnd));
}

void test_SplitAndStripWhitespaceWorksWithStringWithoutWhitespace(void)
{
    char test[] = "DATA_AVERAGING_INTERVAL_SECS=1";

    char * pLStart;
    char * pLEnd;
    char * pRStart;
    char * pREnd;

    TEST_ASSERT_TRUE(splitAndStripWhiteSpace(test, '=', &pLStart, &pLEnd, &pRStart, &pREnd));

    char expected1[] = "DATA_AVERAGING_INTERVAL_SECS";
    char expected2[] = "";
    char expected3[] = "1";
    char expected4[] = "";
    TEST_ASSERT_EQUAL(0, strncmp(expected1, pLStart, strlen(expected1)));
    TEST_ASSERT_EQUAL(0, strncmp(expected2, pLEnd+1, strlen(expected2)));
    TEST_ASSERT_EQUAL(0, strncmp(expected3, pRStart, strlen(expected3)));
    TEST_ASSERT_EQUAL(0, strncmp(expected4, pREnd+1, strlen(expected4)));
}

void test_SplitAndStripWhitespaceWorksWithStringWithWhitespace(void)
{
    char test[] = "DATA_AVERAGING_INTERVAL_SECS = 1";

    char * pLStart;
    char * pLEnd;
    char * pRStart;
    char * pREnd;

    TEST_ASSERT_TRUE(splitAndStripWhiteSpace(test, '=', &pLStart, &pLEnd, &pRStart, &pREnd));

    char expected1[] = "DATA_AVERAGING_INTERVAL_SECS";
    char expected2[] = "";
    char expected3[] = "1";
    char expected4[] = "";
    TEST_ASSERT_EQUAL(0, strncmp(expected1, pLStart, strlen(expected1)));
    TEST_ASSERT_EQUAL(0, strncmp(expected2, pLEnd+1, strlen(expected2)));
    TEST_ASSERT_EQUAL(0, strncmp(expected3, pRStart, strlen(expected3)));
    TEST_ASSERT_EQUAL(0, strncmp(expected4, pREnd+1, strlen(expected4)));
}

//=======MAIN=====
int main(void)
{
  UnityBegin("DLUtility.Strings.cpp");
  RUN_TEST(test_toLowerStr_DoesNothingToEmptyString);
  RUN_TEST(test_toLowerStr_DoesNothingToLowerCaseString);
  RUN_TEST(test_toLowerStr_CorrectlyLowercasesMixedCaseString);
  
  RUN_TEST(test_strncpy_safe_CorrectlyCopiesStringLessThanMaxLength);
  RUN_TEST(test_strncpy_safe_CorrectlyCopiesStringEqualToThanMaxLength);
  RUN_TEST(test_strncpy_safe_CorrectlyCopiesStringGreaterThanMaxLength);
  RUN_TEST(test_strncpy_safe_CorrectlyCopiesStringOfRequestedLength);
  RUN_TEST(test_strncpy_safe_CorrectlyCopiesAllOfSourseWhenRequestingMoreThanLength);
 
  RUN_TEST(test_skipSpacesCorrectlyJumpsOverWhitespace);

  RUN_TEST(test_stringIsWhitespaceReturnsTrueForWhitespaceOrBlankAndTrueOtherwise);

  RUN_TEST(test_FixedLengthAccumulator_InitsCorrectly);
  RUN_TEST(test_FixedLengthAccumulator_FillsUpToMaxLength);
  RUN_TEST(test_FixedLengthAccumulator_ResetsCorrectly);
  RUN_TEST(test_FixedLengthAccumulator_AcceptStringLessThanMaxLength);
  RUN_TEST(test_FixedLengthAccumulator_CopiesPartialStringUpToMaxLength);
  RUN_TEST(test_FixedLengthAccumulator_WritesCRLFUsingWriteLine);
  RUN_TEST(test_FixedLengthAccumulator_CorrectlyReturnsCurrentLength);
  RUN_TEST(test_FixedLengthAccumulator_CorrectlyRemovesAndAddsNewContent);

  RUN_TEST(test_SplitAndStripWhitespaceErrorsWithInvalidStrings);
  RUN_TEST(test_SplitAndStripWhitespaceWorksWithStringWithoutWhitespace);
  RUN_TEST(test_SplitAndStripWhitespaceWorksWithStringWithWhitespace);

  return (UnityEnd());
}