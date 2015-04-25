/*
 * C++ Library Includes
 */

#include <stdint.h>

#include "DLSensor.ADS1x1x.h"
#include "DLTest.Mock.random.h"

/**************************************************************************/
/*!
@brief  Instantiates a new ADS1x1x class w/appropriate properties
*/
/**************************************************************************/
ADS1x1x::ADS1x1x(uint8_t i2cAddress) 
{
    // Set defaults for the lowest performance ADC (ADS1013)
    m_i2cAddress = i2cAddress;
    m_gain = GAIN_TWOTHIRDS; /* +/- 6.144V range (limited to VDD +0.3V max!) */

    m_minFakeRead[0] = m_minFakeRead[1] = m_minFakeRead[2] = m_minFakeRead[3] = 0;
    m_maxFakeRead[0] = m_maxFakeRead[1] = m_maxFakeRead[2] = m_maxFakeRead[3] = 1;
}

ADS1013::ADS1013(uint8_t i2cAddress) : ADS1x1x(i2cAddress) {}
ADS1014::ADS1014(uint8_t i2cAddress) : ADS1x1x(i2cAddress) {}
ADS1015::ADS1015(uint8_t i2cAddress) : ADS1x1x(i2cAddress) {}
ADS1113::ADS1113(uint8_t i2cAddress) : ADS1x1x(i2cAddress) {}
ADS1114::ADS1114(uint8_t i2cAddress) : ADS1x1x(i2cAddress) {}
ADS1115::ADS1115(uint8_t i2cAddress) : ADS1x1x(i2cAddress) {}

void ADS1x1x::begin()
{
}

void ADS1x1x::fake(uint8_t ch, uint16_t minFakeRead, uint16_t maxFakeRead)
{
    m_minFakeRead[ch] = minFakeRead;
    m_maxFakeRead[ch] = maxFakeRead;
}

void ADS1x1x::setGain(ADS_GAIN gain)
{
    m_gain = gain;
}

ADS_GAIN ADS1x1x::getGain()
{
    return m_gain;
}

uint8_t ADS1x1x::getAddress()
{
    return m_i2cAddress;
}

int16_t ADS1x1x::readADC_SingleEnded(uint8_t channel)
{
    return random(m_minFakeRead[channel], m_maxFakeRead[channel]);
}

int16_t ADS1x1x::readADC_Differential(uint8_t channel)
{
    return random(m_minFakeRead[channel], m_maxFakeRead[channel]);
}

int16_t ADS1015::readADC_Differential(uint8_t channel)
{
    return random(m_minFakeRead[channel], m_maxFakeRead[channel]);
}

int16_t ADS1115::readADC_Differential(uint8_t channel)
{
    return random(m_minFakeRead[channel], m_maxFakeRead[channel]);
}

int16_t ADS1x1x::readADC_DifferentialWithConfig(uint16_t differentialConfig)
{
    (void)differentialConfig;
    return random(m_minFakeRead[0], m_maxFakeRead[1]);
}

int16_t ADS1x1x::getLastConversionResults()
{
	return random(m_minFakeRead[0], m_maxFakeRead[0]);
}

void ADS1x1x::startComparator_SingleEnded(uint8_t channel, int16_t threshold)
{
	(void)channel; (void)threshold;
}