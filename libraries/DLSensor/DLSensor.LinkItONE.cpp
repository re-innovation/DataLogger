/*
* DLSensor.LinkItONE.cpp
* 
* Interface for the LinkItONE onboard ADCs
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
#include <Wire.h>
#else
#include <stdint.h>
#endif

/*
* Local Application Includes
*/

#include "DLSensor.LinkItONE.h"

/*
* Defines and Typedefs
*/

LinkItONEADC::LinkItONEADC(uint8_t channel)
{
    m_channel = channel;
}

uint16_t LinkItONEADC::read(void)
{
    return analogRead(m_channel);
}
