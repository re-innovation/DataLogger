/*
 * DLDataField.Test.cpp
 * 
 * Tests the DataField class
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
 * Local Application Includes
 */

#include "../DLDataField.h"

/*
 * Unity Test Framework
 */

#include "unity.h"

static int16_t intDataArray[] = {-5, -4, -3, -2, -1, 0, 1, 2, 3, 4};
static char strDataArray[][5] = {"N","NE", "E", "SE", "S"};

static void fillWithTestIntData(NumericDataField<int16_t> * pDataField)
{
	uint8_t i;
	for (i = 0; i < 10; ++i)
	{
		pDataField->storeData(intDataArray[i]);
	}
}

static void fillWithTestStringData(StringDataField * pDataField)
{
	uint8_t i;
	for (i = 0; i < 5; ++i)
	{
		pDataField->storeData(strDataArray[i]);
	}
}

static void test_CreateDataFieldWithCorrectType_ReturnsCorrectTypeAndDefaultValue(void)
{
	static NumericDataField<int16_t> dataField = NumericDataField<int16_t>(VOLTAGE, 1);
	TEST_ASSERT_EQUAL(VOLTAGE, dataField.getType());
	TEST_ASSERT_EQUAL(0, dataField.getData(0));
}

static void test_DatafieldStoreFloats_CorrectlyFormattedAsStrings(void)
{
	static NumericDataField<float> dataField = NumericDataField<float>(VOLTAGE, 1);
	static char buffer[20];
	dataField.storeData(100.12345f);
	
	dataField.getDataAsString(buffer, "%.0f", 0);
	TEST_ASSERT_EQUAL_STRING("100", buffer);

	dataField.getDataAsString(buffer, "%.5f", 0);
	TEST_ASSERT_EQUAL_STRING("100.12345", buffer);
}

static void test_DatafieldStoreAsString_ReturnsZeroLengthStringAsDefaultValue(void)
{
	static StringDataField dataField = StringDataField(CARDINAL_DIRECTION, 20, 1);
	static char buffer[20];
	dataField.copy(buffer, 0);
	TEST_ASSERT_EQUAL(0, strlen(buffer));
}

static void test_DatafieldStoreAsStringThenSet_ReturnsSameString(void)
{
	static StringDataField dataField = StringDataField(CARDINAL_DIRECTION, 20, 1);
	static char buffer[20];
	dataField.storeData((char *)"TESTSTRING");
	dataField.copy(buffer, 0);
	TEST_ASSERT_EQUAL_STRING("TESTSTRING", buffer);
}

static void test_DatafieldStoreAsString_ClipsStringsLongerThanMaximumLength(void)
{
	static StringDataField dataField = StringDataField(CARDINAL_DIRECTION, 20, 1);
	static char buffer[21] = "01234567890123456789";
	static char expectedStr[21];
	static char testStr[] = "THIS IS A REALLY LONG STRING THAT SHOULD BE CLIPPED!";
	dataField.storeData((char*)testStr);
	dataField.copy(buffer, 0);
	strncpy(expectedStr, testStr, 20);
	TEST_ASSERT_EQUAL(20, strlen(buffer));
	TEST_ASSERT_EQUAL_STRING(expectedStr, buffer);	
}

static void test_DatafieldStoreArrayOfInts_CorrectlyStoresInts(void)
{
	NumericDataField<int16_t> dataField = NumericDataField<int16_t>(VOLTAGE, 10);
	fillWithTestIntData(&dataField);

	int16_t i;
	for (i = 0; i < 10; ++i)
	{
		TEST_ASSERT_EQUAL(intDataArray[i], dataField.getData(i));
	}
}

static void test_DatafieldStoreArrayOfStrings_CorrectlyStoresStrings(void)
{
	StringDataField dataField = StringDataField(CARDINAL_DIRECTION, 3, 10);
	fillWithTestStringData(&dataField);

	int16_t i;
	for (i = 0; i < 5; ++i)
	{
		TEST_ASSERT_EQUAL_STRING(strDataArray[i], dataField.getData(i));
	}
}


int main(void)
{
  UnityBegin("DLDataField.cpp");

  RUN_TEST(test_CreateDataFieldWithCorrectType_ReturnsCorrectTypeAndDefaultValue);
  RUN_TEST(test_DatafieldStoreFloats_CorrectlyFormattedAsStrings);
  RUN_TEST(test_DatafieldStoreAsString_ReturnsZeroLengthStringAsDefaultValue);
  RUN_TEST(test_DatafieldStoreAsStringThenSet_ReturnsSameString);
  RUN_TEST(test_DatafieldStoreAsString_ClipsStringsLongerThanMaximumLength);

  RUN_TEST(test_DatafieldStoreArrayOfInts_CorrectlyStoresInts);
  RUN_TEST(test_DatafieldStoreArrayOfStrings_CorrectlyStoresStrings);

  UnityEnd();
  return 0;
}
