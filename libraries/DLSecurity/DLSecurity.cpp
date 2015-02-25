/*
 * DLSecurity.cpp
 * 
 * Implements security features for the datalogger
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
#include <iostream>
#endif


/*
 * Local Application Includes
 */

#include "DLUtility.Location.h"
#include "DLSecurity.h"

/*
 * Alarm class : Public Functions 
 */

Alarm::Alarm(ON_ALARM_TRIPPED_FN pOnTripFn)
{
    m_pOnTripped = pOnTripFn;
}

Alarm::~Alarm() {}

/*
 * TripAlarm class : Public Functions 
 */

TripAlarm::TripAlarm(ON_ALARM_TRIPPED_FN pOnTripFn) : Alarm(pOnTripFn) {}
TripAlarm::~TripAlarm() {}
void TripAlarm::update(bool tripped)
{
    if (tripped && m_pOnTripped)
    {
        m_pOnTripped();
    }
}

/*
 * LocationAlarm class : Public Functions 
 */

LocationAlarm::LocationAlarm(ON_ALARM_TRIPPED_FN pOnTripFn, uint32_t alarmDistance, LOCATION_2D * pLocation) : Alarm(pOnTripFn)
{
    setHome(pLocation);
    m_alarmDistance = alarmDistance;
}
LocationAlarm::~LocationAlarm() {}

void LocationAlarm::setHome(LOCATION_2D * pLocation)
{
    if (pLocation)
    {
        m_home.latitude = pLocation->latitude;
        m_home.longitude = pLocation->longitude;
    }
}

void LocationAlarm::setAlarmDistance(uint32_t newDistance)
{
    m_alarmDistance = newDistance;
}

void LocationAlarm::update(LOCATION_2D * pLocation)
{
    uint32_t distanceFromHome = Location_diffInMeters(pLocation, &m_home);
    std::cout << distanceFromHome <<std::endl;
    if (m_alarmDistance < distanceFromHome)
    {
        if(m_pOnTripped)
        {
            m_pOnTripped();
        }
    }
}
