#ifndef _GPS_H_
#define _GPS_H_

/*
 * These functions must be supported by each underlying platform
 */

/* 
 * GPS_Setup
 *  Configures the GPS hardware
 * Args:
 *  updatePeriodSeconds: If > 0, the GPS position and time will update at this period.
 */

bool GPS_Setup(uint32_t updatePeriodSeconds);

/* 
 * GPS_Tick
 *  This function MUST be called by the application in order to perform timed GPS updates.
 * Args:
 *  None
 */

void GPS_Tick(void);

/* 
 * GPS_UpdateNow
 *  This function can be called to force an update of location and time.
 *  If timed updates are off, then this is the only way to update.
 * Args:
 *  None
 */

void GPS_UpdateNow(void);

/* 
 * GPS_EnableUpdates
 *  Disables or enables automatic position updates.
 *  Will only enable updates if the timing period is > 0.
 *  If timed updates are off, then this is the only way to update.
 * Args:
 *  None
 */
 
void GPS_EnableUpdates(bool enable);

void GPS_SetUpdatePeriod(uint32_t newPeriod);

bool GPS_GetLocation_2D(LOCATION_2D * pLocation);
bool GPS_GetLocation_3D(LOCATION_3D * pLocation);

uint32_t GPS_GetGPSTime(void);
        
#endif

