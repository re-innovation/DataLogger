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

static void test_DatafieldStoreArrayOfInts_BehavesAsCircularBuffer(void)
{
	NumericDataField<int16_t> dataField = NumericDataField<int16_t>(VOLTAGE, 10);
	fillWithTestIntData(&dataField);

	dataField.storeData(5);
	dataField.storeData(6);
	dataField.storeData(7);

	int16_t i;
	// Datafield should have stored last 7 points in data array
	for (i = 0; i < 7; ++i)
	{
		TEST_ASSERT_EQUAL(intDataArray[i+3], dataField.getData(i));
	}

	// Newest three additions should be in last three entries
	TEST_ASSERT_EQUAL(5, dataField.getData(7));
	TEST_ASSERT_EQUAL(6, dataField.getData(8));
	TEST_ASSERT_EQUAL(7, dataField.getData(9));
}

static void test_DatafieldStoreArrayOfStrings_CorrectlyStoresStrings(void)
{
	StringDataField dataField = StringDataField(CARDINAL_DIRECTION, 3, 5);
	fillWithTestStringData(&dataField);

	int16_t i;
	for (i = 0; i < 5; ++i)
	{
		TEST_ASSERT_EQUAL_STRING(strDataArray[i], dataField.getData(i));
	}
}

static void test_DatafieldStoreArrayOfStrings_BehavesAsCircularBuffer(void)
{
	StringDataField dataField = StringDataField(CARDINAL_DIRECTION, 3, 5);
	fillWithTestStringData(&dataField);

	dataField.storeData((char*)"SW");
	dataField.storeData((char*)"W");
	dataField.storeData((char*)"NW");

	int16_t i;
	// Datafield should have stored last 2 points in data array
	for (i = 0; i < 2; ++i)
	{
		TEST_ASSERT_EQUAL_STRING(strDataArray[i+3], dataField.getData(i));
	}

	// Newest three additions should be in last three entries
	TEST_ASSERT_EQUAL_STRING("SW", dataField.getData(2));
	TEST_ASSERT_EQUAL_STRING("W", dataField.getData(3));
	TEST_ASSERT_EQUAL_STRING("NW", dataField.getData(4));
}

static void test_GetFieldTypeString_ReturnsStringforValidIndexAndEmptyOtherwise(void)
{
	StringDataField dataField = StringDataField(DEGREES_DIRECTION, 3, 5);
	TEST_ASSERT_EQUAL_STRING("Wind Direction", dataField.getTypeString());

	StringDataField invalidDataField = StringDataField((FIELD_TYPE)(DEGREES_DIRECTION+1), 3, 5);
	TEST_ASSERT_EQUAL_STRING("", invalidDataField.getTypeString());
}

static void test_DatafieldStoreArrayOfInts_CorrectlyHandlesIndexesGreaterThanLength(void)
{
	NumericDataField<int16_t> dataField = NumericDataField<int16_t>(VOLTAGE, 10);
	fillWithTestIntData(&dataField);

	TEST_ASSERT_EQUAL(-3, dataField.getData(98357952));
}

static void test_DatafieldStoreArrayOfStrings_CorrectlyHandlesIndexesGreaterThanLength(void)
{
	StringDataField dataField = StringDataField(CARDINAL_DIRECTION, 3, 5);
	fillWithTestStringData(&dataField);

	TEST_ASSERT_EQUAL_STRING("E", dataField.getData(98357957));
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
    RUN_TEST(test_DatafieldStoreArrayOfInts_BehavesAsCircularBuffer);
    RUN_TEST(test_DatafieldStoreArrayOfInts_CorrectlyHandlesIndexesGreaterThanLength);
    
    RUN_TEST(test_DatafieldStoreArrayOfStrings_CorrectlyStoresStrings);
    RUN_TEST(test_DatafieldStoreArrayOfStrings_BehavesAsCircularBuffer);
	RUN_TEST(test_DatafieldStoreArrayOfStrings_CorrectlyHandlesIndexesGreaterThanLength);
    
    RUN_TEST(test_GetFieldTypeString_ReturnsStringforValidIndexAndEmptyOtherwise);

    UnityEnd();
    return 0;
}
