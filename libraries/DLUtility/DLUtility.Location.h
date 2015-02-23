#ifndef _UTILITY_LOCATION_H_
#define _UTILITY_LOCATION_H_

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

#endif
