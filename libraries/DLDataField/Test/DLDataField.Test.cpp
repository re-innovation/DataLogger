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

#include <string.h>

/*
 * Local Application Includes
 */

#include "../DLDataField.h"

/*
 * Unity Test Framework
 */

#include "unity.h"

static void test_CreateDataFieldWithCorrectType_ReturnsCorrectTypeAndDefaultValue(void)
{
	static DataField dataField = DataField(VOLTAGE);
	TEST_ASSERT_EQUAL(VOLTAGE, dataField.GetType());
	TEST_ASSERT_EQUAL(0.0f, dataField.GetDataAsFloat());

	dataField = DataField(CURRENT);
	TEST_ASSERT_EQUAL(CURRENT, dataField.GetType());
	TEST_ASSERT_EQUAL(0.0f, dataField.GetDataAsFloat());
}

static void test_DatafieldStoreAsDifferentNumericTypes_ThenGetAsFloatWorks(void)
{
	static DataField dataField = DataField(VOLTAGE);
	
	dataField.StoreData(100.0f);
	TEST_ASSERT_EQUAL(100.0f, dataField.GetDataAsFloat());

	dataField.StoreData((uint8_t)100);
	TEST_ASSERT_EQUAL(100.0f, dataField.GetDataAsFloat());
	dataField.StoreData((int8_t)100);
	TEST_ASSERT_EQUAL(100.0f, dataField.GetDataAsFloat());
	dataField.StoreData((uint16_t)100);
	TEST_ASSERT_EQUAL(100.0f, dataField.GetDataAsFloat());
	dataField.StoreData((int16_t)100);
	TEST_ASSERT_EQUAL(100.0f, dataField.GetDataAsFloat());
	dataField.StoreData((uint32_t)100);
	TEST_ASSERT_EQUAL(100.0f, dataField.GetDataAsFloat());
	dataField.StoreData((int32_t)100);
	TEST_ASSERT_EQUAL(100.0f, dataField.GetDataAsFloat());
}

static void test_DatafieldStoreFloats_CorrectlyFormattedAsStrings(void)
{
	static DataField dataField = DataField(VOLTAGE);
	static char buffer[20];
	dataField.StoreData(100.12345f);
	
	dataField.GetDataAsString(buffer, "%.0f");
	TEST_ASSERT_EQUAL_STRING("100", buffer);

	dataField.GetDataAsString(buffer, "%.5f");
	TEST_ASSERT_EQUAL_STRING("100.12345", buffer);
}

static void test_DatafieldStoreAsString_ReturnsZeroLengthStringAsDefaultValue(void)
{
	static DataField dataField = DataField(GENERIC_STRING);
	static char buffer[20];
	dataField.GetDataAsString(buffer, "%s");
	TEST_ASSERT_EQUAL(0, strlen(buffer));
}

static void test_DatafieldStoreAsStringThenSet_ReturnsSameString(void)
{
	static DataField dataField = DataField(GENERIC_STRING);	
	static char buffer[20];
	dataField.StoreData("TESTSTRING");
	dataField.GetDataAsString(buffer, "%s");
	TEST_ASSERT_EQUAL_STRING("TESTSTRING", buffer);
}

static void test_DatafieldStoreAsString_ClipsStringsLongerThanMaximumLength(void)
{
	static DataField dataField = DataField(GENERIC_STRING);	
	static char buffer[20];
	static char expectedStr[20];
	static char testStr[] = "THIS IS A REALLY LONG STRING THAT SHOULD BE CLIPPED!";
	dataField.StoreData(testStr);
	dataField.GetDataAsString(buffer, "%s");
	strncpy(expectedStr, testStr, MAX_DATAFIELD_STRING_SIZE);
	TEST_ASSERT_EQUAL(MAX_DATAFIELD_STRING_SIZE, strlen(buffer));
	TEST_ASSERT_EQUAL_STRING(expectedStr, buffer);	
}

static void test_DatafieldSetTypeAsStringAndSetFloatValue_ReturnsZeroAsFloatValue(void)
{
	static DataField dataField = DataField(GENERIC_STRING);
	dataField.StoreData(100.f);
	TEST_ASSERT_EQUAL(0.0f, dataField.GetDataAsFloat());
}

int main(void)
{
  UnityBegin("DLDataField.cpp");

  RUN_TEST(test_CreateDataFieldWithCorrectType_ReturnsCorrectTypeAndDefaultValue);
  RUN_TEST(test_DatafieldStoreAsDifferentNumericTypes_ThenGetAsFloatWorks);
  RUN_TEST(test_DatafieldStoreFloats_CorrectlyFormattedAsStrings);
  RUN_TEST(test_DatafieldStoreAsString_ReturnsZeroLengthStringAsDefaultValue);
  RUN_TEST(test_DatafieldStoreAsStringThenSet_ReturnsSameString);
  RUN_TEST(test_DatafieldStoreAsString_ClipsStringsLongerThanMaximumLength);
  RUN_TEST(test_DatafieldSetTypeAsStringAndSetFloatValue_ReturnsZeroAsFloatValue);

  UnityEnd();
  return 0;
}
