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
 *  enable
 */
 
void GPS_EnableUpdates(bool enable);

/* 
 * GPS_SetUpdatePeriod
 *  Set the update period in milliseconds.
 *  This will only update the GPS position if updates are enabled.
 *  See GPS_EnableUpdates.
 * Args:
 *  newPeriod - The period in milliseconds (must be >0 or task will not run)
 */
 
void GPS_SetUpdatePeriod(uint32_t newPeriod);

/* 
 * GPS_GetLocation_2D
 *  Get the latest calculated location
 *  If updates are not enabled or GPS_UpdateNow is not called,
 *  this function will return the same information each time.
 * Args:
*  pLocation - Pointer to a location struct to fill
 */
 
bool GPS_GetLocation_2D(LOCATION_2D * pLocation);

/* 
 * GPS_GetLocation_3D
 *  Get the latest calculated location + altitude
 *  If updates are not enabled or GPS_UpdateNow is not called,
 *  this function will return the same information each time.
 * Args:
 *  pLocation - Pointer to a location struct to fill
 */
 
bool GPS_GetLocation_3D(LOCATION_3D * pLocation);

/* 
 * GPS_GetGPSTime
 *  Get the latest GPS time
 *  If updates are not enabled or GPS_UpdateNow is not called,
 *  this function will return the same information each time.
 * Args:
 *  time - Pointer to TM struct to write (see DLUtility.Time.h)
 */

void GPS_GetGPSTime(TM * time);
        
#endif

