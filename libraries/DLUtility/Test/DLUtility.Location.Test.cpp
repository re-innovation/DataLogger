#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <iostream>
#include <iomanip>

#include "unity.h"

#include "../DLUtility.Location.h"

void testSameLocation_ReturnsDifferenceOfZero(void)
{
    LOCATION_2D l1 = {0.0f, 0.0f};
    LOCATION_2D l2 = {-90.f, 180.0f};
    LOCATION_2D l3 = {90.f, 180.0f};
    LOCATION_2D l4 = {90.f, -180.0f};
    LOCATION_2D l5 = {-90.f, -180.0f};
    
    TEST_ASSERT_EQUAL(0, Location_diffInMeters(&l1, &l1));
    TEST_ASSERT_EQUAL(0, Location_diffInMeters(&l2, &l2));
    TEST_ASSERT_EQUAL(0, Location_diffInMeters(&l3, &l3));
    TEST_ASSERT_EQUAL(0, Location_diffInMeters(&l4, &l4));
    TEST_ASSERT_EQUAL(0, Location_diffInMeters(&l5, &l5));
}

void test180DegreesApart_ReturnsDifferenceOfHalfEarthCircumference(void)
{
    // Define nodes
    LOCATION_2D l1[] = {
        {0.0f, 0.0f},   // Intersection of Prime Meridian with equator
        {90.0f, 0.0f},  // North pole
        {0.0f, 90.0f},  // 90E on equator
        {45.0f, 45.0f}  // 45N, 45E
    };
    
    // And anti-nodes on other side of Earth
    LOCATION_2D l2[] = {
        {0.0f, 180.0f},
        {-90.0f, 0.0f},     // South Pole
        {0.0f, -90.0f},     // 90W on equator
        {-45.0f, -315.0f}   // 45S, 315E
    };

    for(uint8_t i = 0; i < 3; i++)
    {
        // 32-bit floats can't get sub-unit precision at this distance, so test with 2m.
        TEST_ASSERT_UINT32_WITHIN(2, MEAN_EARTH_CIRCUMFERENCE_M/2, Location_diffInMeters(&l1[i], &l2[i]));
    }
}

void test1DegreeApart_ReturnsDifferenceOfOneDegreeOfArc(void)
{
    uint32_t oneDegreeOfArc = MEAN_EARTH_RADIUS_M * (PI/180.0f);
    
    LOCATION_2D l1 = {34.234, -76.3};
    LOCATION_2D l2 = {35.234, -76.3};
    
    // Precision at this distance means that within 1m is sufficient
    TEST_ASSERT_UINT32_WITHIN(1, oneDegreeOfArc, Location_diffInMeters(&l1, &l2));
}

//=======MAIN=====
int main(void)
{
  UnityBegin("DLUtility.Location.cpp");
  RUN_TEST(testSameLocation_ReturnsDifferenceOfZero);
  RUN_TEST(test180DegreesApart_ReturnsDifferenceOfHalfEarthCircumference);
  RUN_TEST(test1DegreeApart_ReturnsDifferenceOfOneDegreeOfArc);
  return (UnityEnd());
}