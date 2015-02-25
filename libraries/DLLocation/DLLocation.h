#ifndef _DLLOCATION_H_
#define _DLLOCATION_H_

/*
 * These functions must be supported by each underlying platform
 */

/* 
 * Location_Setup
 *  Configures the GPS hardware
 * Args:
 *  updatePeriodSeconds: If > 0, the GPS position and time will update at this period.
 */

bool Location_Setup(uint32_t updatePeriodSeconds);

/* 
 * Location_Tick
 *  This function MUST be called by the application in order to perform timed GPS updates.
 * Args:
 *  None
 */

void Location_Tick(void);

/* 
 * Location_UpdateNow
 *  This function can be called to force an update of Location and time.
 *  If timed updates are off, then this is the only way to update.
 * Args:
 *  None
 */

void Location_UpdateNow(void);

/* 
 * Location_EnableUpdates
 *  Disables or enables automatic position updates.
 *  Will only enable updates if the timing period is > 0.
 *  If timed updates are off, then this is the only way to update.
 * Args:
 *  enable
 */
 
void Location_EnableUpdates(bool enable);

/* 
 * Location_SetUpdatePeriod
 *  Set the update period in milliseconds.
 *  This will only update the GPS position if updates are enabled.
 *  See Location_EnableUpdates.
 * Args:
 *  newPeriod - The period in milliseconds (must be >0 or task will not run)
 */
 
void Location_SetUpdatePeriod(uint32_t newPeriod);

/* 
 * Location_GetLocation_2D
 *  Get the latest calculated Location
 *  If updates are not enabled or Location_UpdateNow is not called,
 *  this function will return the same information each time.
 * Args:
*  pLocation - Pointer to a Location struct to fill
 */
 
bool Location_GetLocation_2D(Location_2D * pLocation);

/* 
 * Location_GetLocation_3D
 *  Get the latest calculated Location + altitude
 *  If updates are not enabled or Location_UpdateNow is not called,
 *  this function will return the same information each time.
 * Args:
 *  pLocation - Pointer to a Location struct to fill
 */
 
bool Location_GetLocation_3D(Location_3D * pLocation);
        
#endif

