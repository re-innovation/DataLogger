/*
 * DLFilename.cpp
 * 
 * Routines for handling creation of filenames (based on date)
 *
 * Author: James Fowkes
 * Adapted from original software by Matt Little
 *
 * www.re-innovation.co.uk
 */

/*
 * Arduino/C++ Library Includes
 */
 
#ifdef ARDUINO
#include <Arduino.h>
#else
#include <stdint.h>
#endif

/*
 * Local Application Includes
 */

#include "DLFilename.h"
#include "DLUtility.h"
 
static char s_buffer[20];

/* Private Function Definitions */
static bool checkDayIsValid(uint8_t day)
{
    bool valid = (day <= 31);
    return valid;
}

static bool checkMonthIsValid(uint8_t month)
{
    bool valid = (month <= 12);
    return valid;
}

/*
 * Public Functions
 */

void Filename_setFromDate(uint8_t day, uint8_t month, uint8_t year, uint16_t index)
{
    (void)index;

    /* The filename format is Dddmmyy.csv */
    
    if (checkDayIsValid(day) == false) { return; }
    if (checkMonthIsValid(month) == false) { return; }

    year = TWO_DIGIT_YEAR(year); // Ensure year is from 0 to 99
    
    char c = 0;
    s_buffer[c++] = 'D';
    s_buffer[c++] = (year / 10) + '0';
    s_buffer[c++] = (year % 10) + '0';
    
    s_buffer[c++] = '-';
    
    s_buffer[c++] = (month / 10) + '0';
    s_buffer[c++] = (month % 10) + '0';
    
    s_buffer[c++] = '-';
    
    s_buffer[c++] = (day / 10) + '0'; // Get tens and convert to ASCII
    s_buffer[c++] = (day % 10) + '0'; // Get units and convert to ASCII

    //s_buffer[c++] = '-';

    //s_buffer[c++] = (index / 1000) + '0';    
    //index -= (index / 1000) * 1000;
    //s_buffer[c++] = (index / 100) + '0';
    //index -= (index / 100) * 100;
    //s_buffer[c++] = (index / 10) + '0';
    //index -= (index / 10) * 10;
    //s_buffer[c++] = (index) + '0';

    s_buffer[c++] = '.';
    s_buffer[c++] = 'c';
    s_buffer[c++] = 's';
    s_buffer[c++] = 'v';
    s_buffer[c++] = '\0';
}

char const * const Filename_get(void)
{
    return s_buffer;
}

