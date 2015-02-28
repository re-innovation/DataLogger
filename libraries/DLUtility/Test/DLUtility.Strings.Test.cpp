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

//=======MAIN=====
int main(void)
{
  UnityBegin("DLUtility.Strings.cpp");
  RUN_TEST(test_toLowerStr_DoesNothingToEmptyString);
  RUN_TEST(test_toLowerStr_DoesNothingToLowerCaseString);
  RUN_TEST(test_toLowerStr_CorrectlyLowercasesMixedCaseString);
  RUN_TEST(test_FixedLengthAccumulator_InitsCorrectly);
  RUN_TEST(test_FixedLengthAccumulator_FillsUpToMaxLength);
  RUN_TEST(test_FixedLengthAccumulator_ResetsCorrectly);
  RUN_TEST(test_FixedLengthAccumulator_AcceptStringLessThanMaxLength);
  RUN_TEST(test_FixedLengthAccumulator_CopiesPartialStringUpToMaxLength);
  
  return (UnityEnd());
}