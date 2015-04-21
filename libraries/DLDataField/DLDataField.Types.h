#ifndef _DATA_FIELD_TYPES_H_
#define _DATA_FIELD_TYPES_H_

enum field_type
{
    // Electrical values
    VOLTAGE,
    CURRENT,

    // Temperature values
    TEMPERATURE_C,
    TEMPERATURE_F,
    TEMPERATURE_K,
    
    // Solar values
    IRRADIANCE_WpM2,

    // Wind values
    CARDINAL_DIRECTION, //N, NE, E, etc.
    DEGREES_DIRECTION,

    INVALID_TYPE
};
typedef enum field_type FIELD_TYPE;

#endif
