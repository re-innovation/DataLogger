/*
 * DLDataField.ManagerTest.cpp
 * 
 * Tests the DataField manager class
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
#include "DLDataField.h"
#include "DLDataField.Manager.h"

/*
 * Unity Test Framework
 */

#include "unity.h"

static DataFieldManager * s_manager;

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

void setUp(void)
{
    s_manager = new DataFieldManager(10, 10);
}

void test_dataFieldCountStartsAtZero(void)
{
    TEST_ASSERT_EQUAL(0, s_manager->count());
    TEST_ASSERT_EQUAL_PTR(NULL, s_manager->getField(0));
}

void test_stringDataFieldCanBeAdded(void)
{
    StringDataField * field = new StringDataField(CARDINAL_DIRECTION, 10, 10);
    TEST_ASSERT_TRUE(s_manager->addField(field));
    TEST_ASSERT_EQUAL(1, s_manager->count());
    TEST_ASSERT_EQUAL_PTR(field, s_manager->getField(0));
}

void test_numericDataFieldCanBeAdded(void)
{
    NumericDataField * field = new NumericDataField(VOLTAGE, &s_voltageChannelSettings);
    TEST_ASSERT_TRUE(s_manager->addField(field));
    TEST_ASSERT_EQUAL(1, s_manager->count());
    TEST_ASSERT_EQUAL_PTR(field, s_manager->getField(0));
}

void test_getFieldsReturnsPointerToArrayOfFields(void)
{
    NumericDataField * expected[6];

    uint8_t i;
    for (i = 0; i < 6; ++i)
    {
        expected[i] = new NumericDataField(VOLTAGE, &s_voltageChannelSettings);
        s_manager->addField(expected[i]);
    }

    TEST_ASSERT_EQUAL(6, s_manager->count());

    DataField ** dataFields = s_manager->getFields(); 

    for (i = 0; i < 6; ++i)
    {
        TEST_ASSERT_EQUAL_PTR(expected[i], dataFields[i]);
    }
}

static void test_writeHeadersToBufferWritesCorrectFields(void)
{
    s_manager->addField( new NumericDataField(VOLTAGE, &s_voltageChannelSettings) );
    s_manager->addField( new NumericDataField(CURRENT, &s_currentChannelSettings) );
    s_manager->addField( new StringDataField(CARDINAL_DIRECTION, 3, 1) );

    char buffer[100];
    s_manager->writeHeadersToBuffer(buffer, 100);

    TEST_ASSERT_EQUAL_STRING("Voltage (V), Current (A), Wind Direction\r\n", buffer);
}

int main(void)
{
    UnityBegin("DLDataField.Manager.Test.cpp");

    RUN_TEST(test_dataFieldCountStartsAtZero);
    RUN_TEST(test_stringDataFieldCanBeAdded);
    RUN_TEST(test_numericDataFieldCanBeAdded);
    RUN_TEST(test_writeHeadersToBufferWritesCorrectFields);

    UnityEnd();
    return 0;
}
