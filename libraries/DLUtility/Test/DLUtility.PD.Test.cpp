#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "unity.h"

#include "../DLUtility.PD.h"

static void test_GetInputVoltage(void)
{
	static float actual = PD_GetInputVoltage(1.0, 10000, 10000);
	TEST_ASSERT_EQUAL_FLOAT(2.0, actual);

	actual = PD_GetInputVoltage(1.0, 20000, 10000);
	TEST_ASSERT_EQUAL_FLOAT(3.0, actual);
}

static void test_GetLowerResistance(void)
{
	static float actual = PD_GetLowerResistance(10000, 3.3, 3.3/2);
	TEST_ASSERT_EQUAL_FLOAT(10000, actual);	

	actual = PD_GetLowerResistance(10000, 3.3, 3.3*2/3);
	TEST_ASSERT_EQUAL_FLOAT(20000, actual);
}

int main(void)
{
  UnityBegin("DLUtility.PD.cpp");

  RUN_TEST(test_GetInputVoltage);
  RUN_TEST(test_GetLowerResistance);

  return (UnityEnd());
}