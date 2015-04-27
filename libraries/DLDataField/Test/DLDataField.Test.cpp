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

#include "DLUtility.Averager.h"
#include "DLDataField.Types.h"
#include "DLDataField.h"

/*
 * Unity Test Framework
 */

#include "unity.h"

static int32_t s_intDataArray[] = {-5, -4, -3, -2, -1, 0, 20, 40, 60, 80};
static float s_expectedAverage = 18.5f;

static char strDataArray[][5] = {"N","NE", "E", "SE", "S"};

static VOLTAGECHANNEL s_voltageChannelSettings = {
	.mvPerBit = 0.125f,
	.R1 = 200000.0f,
	.R2= 10000.0f,
};

static CURRENTCHANNEL s_currentChannelSettings = {
	.mvPerBit = 0.125f,
	.offset = 60.0f,
	.mvPerAmp = 600.0f,
};

static void fillWithTestIntData(DataField * pDataField)
{
	uint8_t i;
	for (i = 0; i < 10; ++i)
	{
		((NumericDataField*)pDataField)->storeData(s_intDataArray[i]);
	}
}

static void fillWithTestStringData(DataField * pDataField)
{
	uint8_t i;
	for (i = 0; i < 5; ++i)
	{
		((StringDataField*)pDataField)->storeData(strDataArray[i]);
	}
}

static void test_CreateDataFieldWithCorrectType_ReturnsCorrectTypeAndDefaultValue(void)
{
	static NumericDataField dataField = NumericDataField(VOLTAGE, (void*)&s_voltageChannelSettings);
	dataField.setDataSizes(1,1);

	TEST_ASSERT_EQUAL(VOLTAGE, dataField.getType());
	TEST_ASSERT_EQUAL(0, dataField.getRawData(0));
}

static void test_DatafieldStoreInts_CorrectlyFormattedAsStrings(void)
{
	static NumericDataField dataField = NumericDataField(VOLTAGE, (void*)&s_voltageChannelSettings);
	dataField.setDataSizes(1,1);

	static char buffer[20];
	dataField.storeData(100);
	
	dataField.getDataAsString(buffer, "%.0f", 0);
	TEST_ASSERT_EQUAL_STRING("100", buffer);
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
	strncpy(expectedStr, testStr, 19);
	TEST_ASSERT_EQUAL(19, strlen(buffer));
	TEST_ASSERT_EQUAL_STRING(expectedStr, buffer);	
}

static void test_DatafieldStoreArrayOfInts_CorrectlyStoresIntsInAverager(void)
{
	NumericDataField dataField = NumericDataField(VOLTAGE, (void*)&s_voltageChannelSettings);
	dataField.setDataSizes(10, 10);
	
	fillWithTestIntData(&dataField);
	TEST_ASSERT_EQUAL_FLOAT(s_expectedAverage, dataField.getRawData(0));
	TEST_ASSERT_EQUAL_FLOAT(0.0f, dataField.getRawData(1));

	fillWithTestIntData(&dataField);
	TEST_ASSERT_EQUAL_FLOAT(s_expectedAverage, dataField.getRawData(1));
	TEST_ASSERT_EQUAL_FLOAT(0.0f, dataField.getRawData(2));
}

static void test_DatafieldStoreArrayOfInts_CorrectlyReturnsRawAndConvertedData(void)
{
	NumericDataField voltsDataField = NumericDataField(VOLTAGE, (void*)&s_voltageChannelSettings);
	voltsDataField.setDataSizes(10, 10);
	
	fillWithTestIntData(&voltsDataField);
	float expectedConverted = CONV_VoltsFromRaw(s_expectedAverage, &s_voltageChannelSettings);
	TEST_ASSERT_EQUAL_FLOAT(s_expectedAverage, voltsDataField.getRawData(0));
	TEST_ASSERT_EQUAL_FLOAT(expectedConverted, voltsDataField.getConvData(0));

	NumericDataField ampsDataField = NumericDataField(CURRENT, (void*)&s_currentChannelSettings);
	ampsDataField.setDataSizes(10 ,10);
	
	fillWithTestIntData(&ampsDataField);
	expectedConverted = CONV_AmpsFromRaw(s_expectedAverage, &s_currentChannelSettings);
	TEST_ASSERT_EQUAL_FLOAT(s_expectedAverage, ampsDataField.getRawData(0));
	TEST_ASSERT_EQUAL_FLOAT(expectedConverted, ampsDataField.getConvData(0));

}

static void test_DatafieldStoreArrayOfInts_BehavesAsCircularBuffer(void)
{
	NumericDataField dataField = NumericDataField(VOLTAGE, (void*)&s_voltageChannelSettings);
	dataField.setDataSizes(10, 1);
	
	fillWithTestIntData(&dataField);

	dataField.storeData(5);
	dataField.storeData(6);
	dataField.storeData(7);

	int16_t i;
	// Datafield should have stored last 7 points in data array
	for (i = 0; i < 7; ++i)
	{
		TEST_ASSERT_EQUAL_FLOAT((float)s_intDataArray[i+3], dataField.getRawData(i));
	}

	// Newest three additions should be in last three entries
	TEST_ASSERT_EQUAL_FLOAT(5, dataField.getRawData(7));
	TEST_ASSERT_EQUAL_FLOAT(6, dataField.getRawData(8));
	TEST_ASSERT_EQUAL_FLOAT(7, dataField.getRawData(9));
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
	NumericDataField dataField = NumericDataField(VOLTAGE, (void*)&s_voltageChannelSettings);
	dataField.setDataSizes(10, 1);
	
	fillWithTestIntData(&dataField);

	TEST_ASSERT_EQUAL(-3, dataField.getRawData(98357952));
}

static void test_DatafieldStoreArrayOfStrings_CorrectlyHandlesIndexesGreaterThanLength(void)
{
	StringDataField dataField = StringDataField(CARDINAL_DIRECTION, 3, 5);
	fillWithTestStringData(&dataField);

	TEST_ASSERT_EQUAL_STRING("E", dataField.getData(98357957));
}

/*static void test_writeNumericDataFieldsToBuffer_WritesCorrectValues(void)
{
	NumericDataField fieldArray[] = {
		NumericDataField(VOLTAGE, 1),
		NumericDataField(CURRENT, 1),
		NumericDataField(TEMPERATURE_C, 1),
	};

	fieldArray[0].storeData(-13.4533);
	fieldArray[1].storeData(8635.5542);
	fieldArray[2].storeData(34.0);

	char buffer[100];
	DataField_writeNumericDataToBuffer(buffer, fieldArray, "%.2f", 3, 100);

	TEST_ASSERT_EQUAL_STRING("-13.45, 8635.55, 34.00", buffer);
}

static void test_writeStringDataFieldsToBuffer_WritesCorrectValues(void)
{
	StringDataField fieldArray[] = {
		StringDataField(CARDINAL_DIRECTION, 3, 1),
		StringDataField(CARDINAL_DIRECTION, 3, 1),
		StringDataField(CARDINAL_DIRECTION, 3, 1),
	};

	fieldArray[0].storeData("N");
	fieldArray[1].storeData("NE");
	fieldArray[2].storeData("E");

	char buffer[100];
	DataField_writeStringDataToBuffer(buffer, fieldArray, 3, 100);

	TEST_ASSERT_EQUAL_STRING("N, NE, E", buffer);
}*/

int main(void)
{
    UnityBegin("DLDataField.cpp");

    RUN_TEST(test_CreateDataFieldWithCorrectType_ReturnsCorrectTypeAndDefaultValue);
    RUN_TEST(test_DatafieldStoreInts_CorrectlyFormattedAsStrings);
    RUN_TEST(test_DatafieldStoreAsString_ReturnsZeroLengthStringAsDefaultValue);
    RUN_TEST(test_DatafieldStoreAsStringThenSet_ReturnsSameString);
    RUN_TEST(test_DatafieldStoreAsString_ClipsStringsLongerThanMaximumLength);

    RUN_TEST(test_DatafieldStoreArrayOfInts_CorrectlyStoresIntsInAverager);
    RUN_TEST(test_DatafieldStoreArrayOfInts_CorrectlyReturnsRawAndConvertedData);
    RUN_TEST(test_DatafieldStoreArrayOfInts_BehavesAsCircularBuffer);
    RUN_TEST(test_DatafieldStoreArrayOfInts_CorrectlyHandlesIndexesGreaterThanLength);
    
    RUN_TEST(test_DatafieldStoreArrayOfStrings_CorrectlyStoresStrings);
    RUN_TEST(test_DatafieldStoreArrayOfStrings_BehavesAsCircularBuffer);
	RUN_TEST(test_DatafieldStoreArrayOfStrings_CorrectlyHandlesIndexesGreaterThanLength);
    
    RUN_TEST(test_GetFieldTypeString_ReturnsStringforValidIndexAndEmptyOtherwise);

    //RUN_TEST(test_writeNumericDataFieldsToBuffer_WritesCorrectValues);
    //RUN_TEST(test_writeStringDataFieldsToBuffer_WritesCorrectValues);

    UnityEnd();
    return 0;
}
