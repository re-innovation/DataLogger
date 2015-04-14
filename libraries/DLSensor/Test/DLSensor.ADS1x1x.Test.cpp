/*
 * DLSensor.ADS1x1x.Test.cpp
 * 
 * Tests the ADS1x1x class.
 * To make sure that the inheritance tree works,
 * because that particular part of C++ is new to the
 * author.
 *
 * Author: James Fowkes
 *
 * www.re-innovation.co.uk
 */

/*
 * C++ Library Includes
 */

#include <string.h>
#include <stdint.h>
#include <iostream>

/*
 * Local Application Includes
 */

#include "../DLSensor.ADS1x1x.h"
#include "../../DLTest/DLTest.Mock.i2c.h"

/*
 * Private Test Objects/Variables
 */

extern I2CMock Wire;

static uint8_t s_sendBuffer[20];
static uint8_t s_recvdBuffer[20];

/*
 * Unity Test Framework
 */

#include "unity.h"

void setUp()
{
    Wire.setSentBuffer(s_sendBuffer);
    Wire.setRecvdBuffer(s_recvdBuffer);
}

void test_ADS1013_Is12Bit_HasTwoChannels_NoComparator_1msConversionTime()
{
    ADS1013 adc = ADS1013();

    TEST_ASSERT_EQUAL(2, adc.getMaxChannels());
    TEST_ASSERT_EQUAL(1, adc.getConversionTime());
    TEST_ASSERT_EQUAL(12, adc.getResolution());
    TEST_ASSERT_FALSE(adc.hasComparator());
}

void test_ADS1014_Is12Bit_HasTwoChannels_WithComparator_1msConversionTime()
{
    ADS1014 adc = ADS1014();

    TEST_ASSERT_EQUAL(2, adc.getMaxChannels());
    TEST_ASSERT_EQUAL(1, adc.getConversionTime());
    TEST_ASSERT_EQUAL(12, adc.getResolution());
    TEST_ASSERT_TRUE(adc.hasComparator());
}

void test_ADS1015_Is12Bit_HasFourChannels_WithComparator_1msConversionTime()
{
    ADS1015 adc = ADS1015();

    TEST_ASSERT_EQUAL(4, adc.getMaxChannels());
    TEST_ASSERT_EQUAL(1, adc.getConversionTime());
    TEST_ASSERT_EQUAL(12, adc.getResolution());
    TEST_ASSERT_TRUE(adc.hasComparator());
}

void test_ADS1113_Is16Bit_HasTwoChannels_NoComparator_8msConversionTime()
{
    ADS1113 adc = ADS1113();

    TEST_ASSERT_EQUAL(2, adc.getMaxChannels());
    TEST_ASSERT_EQUAL(8, adc.getConversionTime());
    TEST_ASSERT_EQUAL(16, adc.getResolution());
    TEST_ASSERT_FALSE(adc.hasComparator());
}

void test_ADS1114_Is16Bit_HasTwoChannels_WithComparator_8msConversionTime()
{
    ADS1114 adc = ADS1114();

    TEST_ASSERT_EQUAL(2, adc.getMaxChannels());
    TEST_ASSERT_EQUAL(8, adc.getConversionTime());
    TEST_ASSERT_EQUAL(16, adc.getResolution());
    TEST_ASSERT_TRUE(adc.hasComparator());
}

void test_ADS1115_Is16Bit_HasFourChannels_WithComparator_8msConversionTime()
{
    ADS1115 adc = ADS1115();

    TEST_ASSERT_EQUAL(4, adc.getMaxChannels());
    TEST_ASSERT_EQUAL(8, adc.getConversionTime());
    TEST_ASSERT_EQUAL(16, adc.getResolution());
    TEST_ASSERT_TRUE(adc.hasComparator());
}

void test_ADS1x13_WillReadFromChannels0And1()
{
    ADS1113 adc = ADS1113();
    adc.readADC_SingleEnded(0);
    TEST_ASSERT_EQUAL(4, Wire.sentCount());
    TEST_ASSERT_EQUAL(2, Wire.recvdCount());

    adc.readADC_SingleEnded(1);
    TEST_ASSERT_EQUAL(8, Wire.sentCount());
    TEST_ASSERT_EQUAL(4, Wire.recvdCount());
}

void test_ADS1x13_WillNotReadFromChannels2Or3()
{
    ADS1113 adc = ADS1113();
    adc.readADC_SingleEnded(2);
    TEST_ASSERT_EQUAL(0, Wire.sentCount());
    TEST_ASSERT_EQUAL(0, Wire.recvdCount());
    adc.readADC_SingleEnded(3);
    TEST_ASSERT_EQUAL(0, Wire.sentCount());
    TEST_ASSERT_EQUAL(0, Wire.recvdCount());
}

void test_ADS1x14_WillReadFromChannels0And1()
{
    ADS1114 adc = ADS1114();
    adc.readADC_SingleEnded(0);
    TEST_ASSERT_EQUAL(4, Wire.sentCount());
    TEST_ASSERT_EQUAL(2, Wire.recvdCount());

    adc.readADC_SingleEnded(1);
    TEST_ASSERT_EQUAL(8, Wire.sentCount());
    TEST_ASSERT_EQUAL(4, Wire.recvdCount());
}

void test_ADS1x14_WillNotReadFromChannels2Or3()
{
    ADS1114 adc = ADS1114();
    adc.readADC_SingleEnded(2);
    TEST_ASSERT_EQUAL(0, Wire.sentCount());
    TEST_ASSERT_EQUAL(0, Wire.recvdCount());
    adc.readADC_SingleEnded(3);
    TEST_ASSERT_EQUAL(0, Wire.sentCount());
    TEST_ASSERT_EQUAL(0, Wire.recvdCount());
}

void test_ADS1x15_WillReadFromAllChannels()
{
    ADS1115 adc = ADS1115();
    adc.readADC_SingleEnded(0);
    TEST_ASSERT_EQUAL(4, Wire.sentCount());
    TEST_ASSERT_EQUAL(2, Wire.recvdCount());
    adc.readADC_SingleEnded(1);
    TEST_ASSERT_EQUAL(8, Wire.sentCount());
    TEST_ASSERT_EQUAL(4, Wire.recvdCount());
    adc.readADC_SingleEnded(2);
    TEST_ASSERT_EQUAL(12, Wire.sentCount());
    TEST_ASSERT_EQUAL(6, Wire.recvdCount());
    adc.readADC_SingleEnded(3);
    TEST_ASSERT_EQUAL(16, Wire.sentCount());
    TEST_ASSERT_EQUAL(8, Wire.recvdCount());
}

int main(void)
{
    UnityBegin("DLSensor.ADS1x1x.cpp");

    RUN_TEST(test_ADS1013_Is12Bit_HasTwoChannels_NoComparator_1msConversionTime);
    RUN_TEST(test_ADS1014_Is12Bit_HasTwoChannels_WithComparator_1msConversionTime);
    RUN_TEST(test_ADS1015_Is12Bit_HasFourChannels_WithComparator_1msConversionTime);
    RUN_TEST(test_ADS1113_Is16Bit_HasTwoChannels_NoComparator_8msConversionTime);
    RUN_TEST(test_ADS1114_Is16Bit_HasTwoChannels_WithComparator_8msConversionTime);
    RUN_TEST(test_ADS1115_Is16Bit_HasFourChannels_WithComparator_8msConversionTime);

    RUN_TEST(test_ADS1x13_WillReadFromChannels0And1);
    RUN_TEST(test_ADS1x13_WillNotReadFromChannels2Or3);

    RUN_TEST(test_ADS1x14_WillReadFromChannels0And1);
    RUN_TEST(test_ADS1x14_WillNotReadFromChannels2Or3);

    RUN_TEST(test_ADS1x15_WillReadFromAllChannels);

    UnityEnd();
    return 0;
}

