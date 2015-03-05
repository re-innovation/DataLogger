/*
 * DLLocation.GPS.Test.cpp
 * 
 * Tests the GPS functionality
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

/*
 * Local Application Includes
 */

#include "../DLLocation.GPS.h"

/*
 * Unity Test Framework
 */

#include "unity.h"

static char onFailMessage[100];

static void test_HHMMSSStringIsParsedCorrectly(void)
{
    uint8_t expected_h = 0;
    uint8_t expected_m = 0;
    uint8_t expected_s = 0;
    
    uint8_t parsed_h = 0;
    uint8_t parsed_m = 0;
    uint8_t parsed_s = 0;
    
    char toParse[12];
    
    for (expected_h = 0; expected_h < 24; expected_h++)
    {
        for (expected_m = 0; expected_m < 60; expected_m++)
        {
            for (expected_s = 0; expected_s < 60; expected_s++)
            {
                sprintf(toParse, "%02d%02d%02d.000", expected_h, expected_m, expected_s);
                TEST_ASSERT_EQUAL(6, parseHHMMSSTime(toParse, &parsed_h, &parsed_m, &parsed_s));
                TEST_ASSERT_EQUAL(expected_h, parsed_h);
                TEST_ASSERT_EQUAL(expected_m, parsed_m);
                TEST_ASSERT_EQUAL(expected_s, parsed_s);
            }
        }
    }
}

static void test_DDMMYYStringIsParsedCorrectly(void)
{
    uint8_t expected_y = 0;
    uint8_t expected_m = 0;
    uint8_t expected_d = 0;
    
    uint8_t parsed_y = 0;
    uint8_t parsed_m = 0;
    uint8_t parsed_d = 0;
    
    char toParse[6];
    
    for (expected_y = 0; expected_y < 100; expected_y++)
    {
        for (expected_m = 1; expected_m < 13; expected_m++)
        {
            for (expected_d = 0; expected_d < 60; expected_d++)
            {
                sprintf(toParse, "%02d%02d%02d", expected_d, expected_m, expected_y);
                TEST_ASSERT_EQUAL(6, parseDDMMYYDate(toParse, &parsed_d, &parsed_m, &parsed_y));
                TEST_ASSERT_EQUAL(expected_d, parsed_d);
                TEST_ASSERT_EQUAL(expected_m, parsed_m);
                TEST_ASSERT_EQUAL(expected_y, parsed_y);
            }
        }
    }
}

static void test_LatitudeStringIsParsedCorrectly(void)
{
    int8_t lat = 0;
    uint32_t millidegrees = 0;
    
    char toParse[10];
    
    for (lat = 0; lat < 91; lat++)
    {
        for (millidegrees = 0; millidegrees < 60000; millidegrees+=1000)
        {
            sprintf(toParse, "%02d%06.3f", lat, (float)millidegrees/1000.0f);
            toParse[8] = ',';
            float expected = (float)lat + (((float)millidegrees) / 60000.0f);
            float result;
            
            // Parse in Northern hemisphere
            toParse[9] = 'N';
            TEST_ASSERT_EQUAL(10, parseLatitude(toParse, &result));
            
            sprintf(onFailMessage, "String to parse was %s. Result: %f, Expected: %f",
                toParse, result, expected);
            
            TEST_ASSERT_EQUAL_FLOAT_MESSAGE(expected, result, onFailMessage);
            
            // Parse in Southern hemisphere
            toParse[9] = 'S';
            TEST_ASSERT_EQUAL(10, parseLatitude(toParse, &result));
            
            sprintf(onFailMessage, "String to parse was %s. Result: %f, Expected: %f",
                toParse, result, -expected);
            
            TEST_ASSERT_EQUAL_FLOAT_MESSAGE(-expected, result, onFailMessage);
        }
    }
}

static void test_LongitudeStringIsParsedCorrectly(void)
{
    int16_t lon = 0;
    uint32_t millidegrees = 0;
    
    char toParse[11];
    
    for (lon = 0; lon < 180; lon++)
    {
        for (millidegrees = 0; millidegrees < 60000; millidegrees+=1000)
        {
            sprintf(toParse, "%03d%06.3f", lon, (float)millidegrees/1000.0f);
            toParse[9] = ',';
            float expected = (float)lon + (((float)millidegrees) / 60000.0f);
            float result;
            
            // Parse in Eastern hemisphere
            toParse[10] = 'E';
            TEST_ASSERT_EQUAL(11, parseLongitude(toParse, &result));
            
            sprintf(onFailMessage, "String to parse was %s. Result: %f, Expected: %f",
                toParse, result, expected);
            
            TEST_ASSERT_EQUAL_FLOAT_MESSAGE(expected, result, onFailMessage);
            
            // Parse in Western hemisphere
            toParse[10] = 'W';
            TEST_ASSERT_EQUAL(11, parseLongitude(toParse, &result));
            
            sprintf(onFailMessage, "String to parse was %s. Result: %f, Expected: %f",
                toParse, result, -expected);
            
            TEST_ASSERT_EQUAL_FLOAT_MESSAGE(-expected, result, onFailMessage);
        }
    }
}

static void test_completeRMCParsedCorrectly(void)
{
    char rmcToParse[] = "$GPRMC,122331.000,A,5255.4560,N,00112.1977,W,0.000,76.05,050315,,,A*6E";
	
    GPS_DATA data;
    
    TEST_ASSERT_TRUE(GPS_parseGPRMCSentence(rmcToParse, &data));
    
    // Test in sequence in string:
    
    // Time
    TEST_ASSERT_EQUAL(12, data.hour);
    TEST_ASSERT_EQUAL(23, data.min);
    TEST_ASSERT_EQUAL(31, data.sec);
    
    // Latitude
    sprintf(onFailMessage, "Result: %f, Expected: %f", data.latitude, 52.9242);
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(52.9242, data.latitude, onFailMessage);
    
    // Longitude
    sprintf(onFailMessage, "Result: %f, Expected: %f", data.longitude, -1.2033);      
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(-1.2033, data.longitude, onFailMessage);
    
    // Date
    TEST_ASSERT_EQUAL(5, data.dd);
    TEST_ASSERT_EQUAL(3, data.mm);
    TEST_ASSERT_EQUAL(15, data.yy);

}

int main(void)
{
    UnityBegin("DLGPS.Utility.cpp");

    RUN_TEST(test_HHMMSSStringIsParsedCorrectly);
    RUN_TEST(test_DDMMYYStringIsParsedCorrectly);
    RUN_TEST(test_LatitudeStringIsParsedCorrectly);
    RUN_TEST(test_LongitudeStringIsParsedCorrectly);
    RUN_TEST(test_completeRMCParsedCorrectly);
    UnityEnd();
    return 0;
}

