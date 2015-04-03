#ifndef _DLTIME_H_
#define _DLTIME_H_

/*
 * Defines and Typedefs
 */

enum time_type_enum
{
    TIME_PLATFORM,  // Time from local platform (e.g. RTC)
    TIME_GPS,       // Time from GPS source
    TIME_NETWORK,   // Time from network source
};
typedef enum time_type_enum TIME_TYPE;

/*
 * Public Function Prototypes
 */

bool Time_GetTime(TM * tm, TIME_TYPE type);

void Time_SetPlatformTime(TM * tm);

#endif
