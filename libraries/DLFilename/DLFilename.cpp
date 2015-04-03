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
 
#include <Arduino.h>

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

void Filename_setFromDate(uint8_t day, uint8_t month, uint8_t year)
{
    /* The filename format is Dddmmyy.csv */
    
    if (checkDayIsValid(day) == false) { return; }
    if (checkMonthIsValid(month) == false) { return; }

    year = TWO_DIGIT_YEAR(year); // Ensure year is from 0 to 99
    
    s_buffer[0] = 'D';
    s_buffer[1] = (day / 10) + '0'; // Get tens and convert to ASCII
    s_buffer[2] = (day % 10) + '0'; // Get units and convert to ASCII
    s_buffer[3] = (month / 10) + '0';
    s_buffer[4] = (month % 10) + '0';
    s_buffer[5] = (year / 10) + '0';
    s_buffer[6] = (year % 10) + '0';
    s_buffer[7] = '.';
    s_buffer[8] = 'c';
    s_buffer[9] = 's';
    s_buffer[10] = 'v';
    s_buffer[11] = '\0';
}

char const * const Filename_get(void)
{
    return s_buffer;
}

