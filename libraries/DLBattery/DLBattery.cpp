
/*
 * DLBattery.cpp
 *
 * James Fowkes
 *
 * 06 April 2015
 *
 * Battery handling for datalogger applications
 */

/*
 * Arduino Library Includes
 */

#include <arduino.h>
 
/*
 * Standard Library Includes
 */

#include <stdint.h>

#ifdef __LINKIT_ONE__
#include <LBattery.h>
#endif

/*
 * DataLogger Includes
 */

#include "DLBattery.h"
#include "DLError.h"
#include "DLSettings.h"
#include "DLSettings.Global.h"
#include "TaskAction.h"

/*
 * Application Includes
 */

#include "app.sms.h"

/*
 * Private Module Variables
 */

static bool s_debugBattery = false;
static int s_batteryWarnLevel = -1;

/*
 * Private Functions
 */

static int get_level()
{
	#ifdef __LINKIT_ONE__
	return LBattery.level()
	#else
	return 0;
	#endif
}

static bool is_charging()
{
	#ifdef __LINKIT_ONE__
	return LBattery.isCharging()
	#else
	return false;
	#endif
}

static void batteryCheckTaskFn(void)
{
    int batteryLevel = get_level();
    bool charging = is_charging();
    bool battery_low = false;

    if (s_debugBattery)
    {
        Serial.print("Battery level: ");
        Serial.print(batteryLevel);
        Serial.println('%');
    }

    if (s_batteryWarnLevel <= 0) { return; }

    char sms[140];

    battery_low = batteryLevel <= s_batteryWarnLevel;
    Error_Running(ERR_BATT_LEVEL_LOW, battery_low);

    if (battery_low)
    {
        if (charging)
        {
            sprintf(sms, "Low Battery Warning: %d%% (charging)", batteryLevel);
        }
        else
        {
            sprintf(sms, "Low Battery Warning: %d%% (not charging)", batteryLevel);
        }
        APP_SMS_SendMaintenanceMessage(sms);
    }
}
TaskAction batteryCheckTask(batteryCheckTaskFn, 60*60*1000, INFINITE_TICKS);

/*
 * Public FUnctions
 */

void Battery_Set_Debug(bool set)
{
	Serial.println("Turning debugging on for battery functionality.");
	s_debugBattery = set;
}

void Battery_Setup(void)
{
    if (Settings_intIsSet(BATTERY_WARN_LEVEL))
    {
        s_batteryWarnLevel = Settings_getInt(BATTERY_WARN_LEVEL);
        unsigned long intervalInMinutes = 60;  // Default to 1 hour
        
        if (Settings_intIsSet(BATTERY_WARN_INTERVAL_MINUTES))
        {
            intervalInMinutes = Settings_getInt(BATTERY_WARN_INTERVAL_MINUTES);
        }    
        
        batteryCheckTask.SetInterval(intervalInMinutes * 60 * 1000);   
        
        Serial.print("Setting battery warning to ");
        Serial.print(s_batteryWarnLevel);
        Serial.print(" and checking every ");
        Serial.print(intervalInMinutes);
        Serial.println(" minutes.");

        // Call the check task right away in case the battery is below level now
        batteryCheckTaskFn();
    }
    else
    {
        s_batteryWarnLevel = 101; // Set 101% so warning is never issued
    }
}

void Battery_Tick(void)
{
	batteryCheckTask.tick();
}