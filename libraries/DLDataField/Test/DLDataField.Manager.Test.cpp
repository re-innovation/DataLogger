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

#include "../DLDataField.h"
#include "../DLDataField.Manager.h"

/*
 * Unity Test Framework
 */

#include "unity.h"

DataFieldManager * s_manager;

void setUp(void)
{
    s_manager = new DataFieldManager();
}

void test_DataFieldCountStartsAtZero(void)
{
    TEST_ASSERT_EQUAL(0, s_manager->count());
    TEST_ASSERT_EQUAL_PTR(NULL, s_manager->getField(0));
}

void test_StringDataFieldCanBeAdded(void)
{
    StringDataField * field = new StringDataField(CARDINAL_DIRECTION, 10, 10);
    TEST_ASSERT_TRUE(s_manager->addField(field));
    TEST_ASSERT_EQUAL(1, s_manager->count());
    TEST_ASSERT_EQUAL_PTR(field, s_manager->getField(0));
}

void test_NumericDataFieldCanBeAdded(void)
{
    NumericDataField * field = new NumericDataField(VOLTAGE, 10);
    TEST_ASSERT_TRUE(s_manager->addField(field));
    TEST_ASSERT_EQUAL(1, s_manager->count());
    TEST_ASSERT_EQUAL_PTR(field, s_manager->getField(0));
}

void test_GetFieldsReturnsPointerToArrayOfFields(void)
{
    DataField * expected[6];

    uint8_t i;
    for (i = 0; i < 6; ++i)
    {
        expected[i] = new NumericDataField(VOLTAGE, 10);
        s_manager->addField(expected[i]);
    }

    TEST_ASSERT_EQUAL(6, s_manager->count());

    DataField ** dataFields = s_manager->getFields(); 

    for (i = 0; i < 6; ++i)
    {
        TEST_ASSERT_EQUAL_PTR(expected[i], dataFields[i]);
    }
}

int main(void)
{
    UnityBegin("DLDataField.Manager.Test.cpp");

    RUN_TEST(test_DataFieldCountStartsAtZero);
    RUN_TEST(test_StringDataFieldCanBeAdded);
    RUN_TEST(test_NumericDataFieldCanBeAdded);
    RUN_TEST(test_GetFieldsReturnsPointerToArrayOfFields);

    UnityEnd();
    return 0;
}
