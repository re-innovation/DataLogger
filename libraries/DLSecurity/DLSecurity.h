#ifndef _DL_SECURITY_H_
#define _DL_SECURITY_H_

/* 
 * Defines and Typedefs
 */

// When an alarm is tripped, the security module calls an application callback with this signature
typedef void (*ON_ALARM_TRIPPED_FN)(void);

// Generic Alarm class, should not be instantiated directly
class Alarm
{
    protected: // Constructors are protected to ensure this class cannot be instantiated
        Alarm(ON_ALARM_TRIPPED_FN pOnTripFn);
        ~Alarm();
        ON_ALARM_TRIPPED_FN m_pOnTripped;
};

// TripAlarm is a simple on/off alarm
class TripAlarm : protected Alarm
{
    public:    
        TripAlarm(ON_ALARM_TRIPPED_FN pOnTripFn);
        ~TripAlarm();   
        
        void update(bool tripped);
};

// LocationAlarm is an alarm that will trigger when "position from home" exceeds a given distance
class LocationAlarm : Alarm
{
    public:
        LocationAlarm(ON_ALARM_TRIPPED_FN pOnTripFn, uint32_t alarmDistance, LOCATION_2D * pLocation);
        ~LocationAlarm();
        void setAlarmDistance(uint32_t newDistance);
        void setHome(LOCATION_2D * pLocation);
        void update(LOCATION_2D * pLocation);
        
    private:
        LOCATION_2D m_home;
        bool m_homeSet;
        uint32_t m_alarmDistance;
};

#endif