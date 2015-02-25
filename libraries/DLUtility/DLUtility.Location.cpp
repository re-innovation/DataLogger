/*
 * DLUtility.Location.cpp
 * 
 * Provides functionality to handle locations (measure distances etc.)
 *
 * Author: James Fowkes
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
#include <stdio.h>
#include <string.h>
#include <math.h>
#endif

/*
 * Local Application Includes
 */

#include "DLUtility.Location.h"

/*
 * Private Functions 
 */

static float radians(float degrees) { return ((degrees * PI) / 180.f); }

/*
 * Public Functions 
 */

uint32_t Location_diffInMeters(LOCATION_2D * p1, LOCATION_2D * p2)
{
    if (!p1 || !p2) { return 0; }
    
    float lat1 = radians(p1->latitude);
    float lat2 = radians(p2->latitude);
    float dlon = radians(p1->longitude - p2->longitude);
    
    /* This function uses spherical law of cosines to get distance */
    float fDistance = acos( (sin(lat1) * sin(lat2)) + (cos(lat1) * cos(lat2) * cos(dlon)) ) * MEAN_EARTH_RADIUS_M;
    return (uint32_t)(fDistance + 0.5f);
}
