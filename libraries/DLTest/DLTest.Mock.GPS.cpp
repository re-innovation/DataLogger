/*
 * DLTest.Mock.GPS.cpp
 * 
 * Mock functions for GPS
 *
 * Author: James Fowkes
 *
 * www.re-innovation.co.uk
 */

/*
 * C++ Library Includes
 */

#include <stdint.h>
/*
 * Local Application Includes
 */

#include "DLUtility.Time.h"
#include "DLGPS.h"

/*
 * Local object and variables
 */

static GPS_DATA s_parsedInfo;
static bool s_infoIsValid = false;

/*
 * Public Functions 
 */

void GPS_UpdateNow(void)
{
	char sentence[] = "$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A";
    s_infoIsValid = GPS_parseGPRMCSentence(sentence, &s_parsedInfo);
}

GPS_DATA const * GPS_GetInfo(void)
{
    return &s_parsedInfo;
}

bool GPS_InfoIsValid(void)
{
    return s_infoIsValid;
}