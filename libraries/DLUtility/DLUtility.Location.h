#ifndef _UTILITY_LOCATION_H_
#define _UTILITY_LOCATION_H_

/* 
 * Defines and typedefs
 */

#ifndef PI
#define PI 3.14159265359f
#endif

#define MEAN_EARTH_RADIUS_KM 6371U
#define MEAN_EARTH_RADIUS_M (MEAN_EARTH_RADIUS_KM * 1000)
#define MEAN_EARTH_CIRCUMFERENCE_KM ((float)(2 * PI * MEAN_EARTH_RADIUS_KM))
#define MEAN_EARTH_CIRCUMFERENCE_M (MEAN_EARTH_CIRCUMFERENCE_KM * 1000.0f)

struct location_2D_struct
{
    float latitude;     // Latitude from -90 to +90 degrees
    float longitude;    // Longitude from -180 to +180 degrees
};
typedef struct location_2D_struct LOCATION_2D;

struct location_3D_struct
{
    float latitude;     // Latitude from -90 to +90 degrees
    float longitude;    // Longitude from -180 to +180 degrees
    float altitude_m;   // Altitude in meters
};
typedef struct location_3D_struct LOCATION_3D;

uint32_t Location_diffInMeters(LOCATION_2D * p1, LOCATION_2D * p2);

#endif
