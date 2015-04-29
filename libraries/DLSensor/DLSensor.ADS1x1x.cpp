/*
* DLSensor.ADS1x1x.cpp
* 
* Interface for the ADS1x1x rang of i2c ADCs
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

#include "DLSensor.ADS1x1x.h"

#ifdef TEST
#include "DLTest.Mock.i2c.h"
#include "DLTest.Mock.delay.h"
#endif

/*
* Defines and Typedefs
*/

/*=========================================================================
CONVERSION DELAY (in mS)
-----------------------------------------------------------------------*/
#define ADS101x_CONVERSIONDELAY         (1)
#define ADS111x_CONVERSIONDELAY         (8)
/*=========================================================================*/

/*=========================================================================
POINTER REGISTER
-----------------------------------------------------------------------*/
#define ADS1x1x_REG_POINTER_MASK        (0x03)
#define ADS1x1x_REG_POINTER_CONVERT     (0x00)
#define ADS1x1x_REG_POINTER_CONFIG      (0x01)
#define ADS1x1x_REG_POINTER_LOWTHRESH   (0x02)
#define ADS1x1x_REG_POINTER_HITHRESH    (0x03)
/*=========================================================================*/

/*=========================================================================
CONFIG REGISTER
-----------------------------------------------------------------------*/
#define ADS1x1x_REG_CONFIG_OS_MASK      (0x8000)
#define ADS1x1x_REG_CONFIG_OS_SINGLE    (0x8000)  // Write: Set to start a single-conversion
#define ADS1x1x_REG_CONFIG_OS_BUSY      (0x0000)  // Read: Bit = 0 when conversion is in progress
#define ADS1x1x_REG_CONFIG_OS_NOTBUSY   (0x8000)  // Read: Bit = 1 when device is not performing a conversion

#define ADS1x1x_REG_CONFIG_MUX_MASK     (0x7000)
#define ADS1x1x_REG_CONFIG_MUX_DIFF_0_1 (0x0000)  // Differential P = AIN0, N = AIN1 (default)
#define ADS1x1x_REG_CONFIG_MUX_DIFF_0_3 (0x1000)  // Differential P = AIN0, N = AIN3
#define ADS1x1x_REG_CONFIG_MUX_DIFF_1_3 (0x2000)  // Differential P = AIN1, N = AIN3
#define ADS1x1x_REG_CONFIG_MUX_DIFF_2_3 (0x3000)  // Differential P = AIN2, N = AIN3
#define ADS1x1x_REG_CONFIG_MUX_SINGLE_0 (0x4000)  // Single-ended AIN0
#define ADS1x1x_REG_CONFIG_MUX_SINGLE_1 (0x5000)  // Single-ended AIN1
#define ADS1x1x_REG_CONFIG_MUX_SINGLE_2 (0x6000)  // Single-ended AIN2
#define ADS1x1x_REG_CONFIG_MUX_SINGLE_3 (0x7000)  // Single-ended AIN3

#define ADS1x1x_REG_CONFIG_PGA_MASK     (0x0E00)
#define ADS1x1x_REG_CONFIG_PGA_6_144V   (0x0000)  // +/-6.144V range = Gain 2/3
#define ADS1x1x_REG_CONFIG_PGA_4_096V   (0x0200)  // +/-4.096V range = Gain 1
#define ADS1x1x_REG_CONFIG_PGA_2_048V   (0x0400)  // +/-2.048V range = Gain 2 (default)
#define ADS1x1x_REG_CONFIG_PGA_1_024V   (0x0600)  // +/-1.024V range = Gain 4
#define ADS1x1x_REG_CONFIG_PGA_0_512V   (0x0800)  // +/-0.512V range = Gain 8
#define ADS1x1x_REG_CONFIG_PGA_0_256V   (0x0A00)  // +/-0.256V range = Gain 16

#define ADS1x1x_REG_CONFIG_MODE_MASK    (0x0100)
#define ADS1x1x_REG_CONFIG_MODE_CONTIN  (0x0000)  // Continuous conversion mode
#define ADS1x1x_REG_CONFIG_MODE_SINGLE  (0x0100)  // Power-down single-shot mode (default)

#define ADS1x1x_REG_CONFIG_DR_MASK      (0x00E0)  
#define ADS1x1x_REG_CONFIG_DR_128SPS    (0x0000)  // 128 samples per second
#define ADS1x1x_REG_CONFIG_DR_250SPS    (0x0020)  // 250 samples per second
#define ADS1x1x_REG_CONFIG_DR_490SPS    (0x0040)  // 490 samples per second
#define ADS1x1x_REG_CONFIG_DR_920SPS    (0x0060)  // 920 samples per second
#define ADS1x1x_REG_CONFIG_DR_1600SPS   (0x0080)  // 1600 samples per second (default)
#define ADS1x1x_REG_CONFIG_DR_2400SPS   (0x00A0)  // 2400 samples per second
#define ADS1x1x_REG_CONFIG_DR_3300SPS   (0x00C0)  // 3300 samples per second

#define ADS1x1x_REG_CONFIG_CMODE_MASK   (0x0010)
#define ADS1x1x_REG_CONFIG_CMODE_TRAD   (0x0000)  // Traditional comparator with hysteresis (default)
#define ADS1x1x_REG_CONFIG_CMODE_WINDOW (0x0010)  // Window comparator

#define ADS1x1x_REG_CONFIG_CPOL_MASK    (0x0008)
#define ADS1x1x_REG_CONFIG_CPOL_ACTVLOW (0x0000)  // ALERT/RDY pin is low when active (default)
#define ADS1x1x_REG_CONFIG_CPOL_ACTVHI  (0x0008)  // ALERT/RDY pin is high when active

#define ADS1x1x_REG_CONFIG_CLAT_MASK    (0x0004)  // Determines if ALERT/RDY pin latches once asserted
#define ADS1x1x_REG_CONFIG_CLAT_NONLAT  (0x0000)  // Non-latching comparator (default)
#define ADS1x1x_REG_CONFIG_CLAT_LATCH   (0x0004)  // Latching comparator

#define ADS1x1x_REG_CONFIG_CQUE_MASK    (0x0003)
#define ADS1x1x_REG_CONFIG_CQUE_1CONV   (0x0000)  // Assert ALERT/RDY after one conversions
#define ADS1x1x_REG_CONFIG_CQUE_2CONV   (0x0001)  // Assert ALERT/RDY after two conversions
#define ADS1x1x_REG_CONFIG_CQUE_4CONV   (0x0002)  // Assert ALERT/RDY after four conversions
#define ADS1x1x_REG_CONFIG_CQUE_NONE    (0x0003)  // Disable the comparator and put ALERT/RDY in high state (default)

/*=========================================================================*/

/*
* Private Variables
*/

static uint16_t s_gainSettings[] = {
    // Values align with ADS_GAIN enumeration
    ADS1x1x_REG_CONFIG_PGA_6_144V,
    ADS1x1x_REG_CONFIG_PGA_4_096V,
    ADS1x1x_REG_CONFIG_PGA_2_048V,
    ADS1x1x_REG_CONFIG_PGA_1_024V,
    ADS1x1x_REG_CONFIG_PGA_0_512V,
    ADS1x1x_REG_CONFIG_PGA_0_256V
};

static const uint16_t s_channels[] = {
    ADS1x1x_REG_CONFIG_MUX_SINGLE_0,
    ADS1x1x_REG_CONFIG_MUX_SINGLE_1,
    ADS1x1x_REG_CONFIG_MUX_SINGLE_2, // Only available on 1x15 parts
    ADS1x1x_REG_CONFIG_MUX_SINGLE_3  // Only available on 1x15 parts
};

static const uint16_t s_differentialChannels[] = {
    ADS1x1x_REG_CONFIG_MUX_DIFF_0_1,
    ADS1x1x_REG_CONFIG_MUX_DIFF_2_3,
};

static const uint16_t ADS1x1x_DEFAULT_ADC_CONFIG = 
    ADS1x1x_REG_CONFIG_CQUE_NONE    | // Disable the comparator (default val)
    ADS1x1x_REG_CONFIG_CLAT_NONLAT  | // Non-latching (default val)
    ADS1x1x_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
    ADS1x1x_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
    ADS1x1x_REG_CONFIG_DR_1600SPS   | // 1600 samples per second (default)
    ADS1x1x_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default))

static const uint16_t ADS1x1x_DEFAULT_COMPARATOR_CONFIG = 
    ADS1x1x_REG_CONFIG_CQUE_1CONV   | // Comparator enabled and asserts on 1 match
    ADS1x1x_REG_CONFIG_CLAT_LATCH   | // Latching mode
    ADS1x1x_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
    ADS1x1x_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
    ADS1x1x_REG_CONFIG_DR_1600SPS   | // 1600 samples per second (default)
    ADS1x1x_REG_CONFIG_MODE_CONTIN  | // Continuous conversion mode
    ADS1x1x_REG_CONFIG_MODE_CONTIN;   // Continuous conversion mode

#ifdef TEST
extern I2CMock Wire;
#endif

/*
* Private Functions 
*/

/**************************************************************************/
/*!
@brief  Abstract away platform differences in Arduino wire library
*/
/**************************************************************************/
static uint8_t i2cread(void) {
#if ARDUINO >= 100
    return Wire.read();
#else
    return Wire.receive();
#endif
}

/**************************************************************************/
/*!
@brief  Abstract away platform differences in Arduino wire library
*/
/**************************************************************************/
static void i2cwrite(uint8_t x) {
#if ARDUINO >= 100
    Wire.write((uint8_t)x);
#else
    Wire.send(x);
#endif
}

/**************************************************************************/
/*!
@brief  Writes 16-bits to the specified destination register
*/
/**************************************************************************/
static void writeRegister(uint8_t i2cAddress, uint8_t reg, uint16_t value) {
    Wire.beginTransmission(i2cAddress);
    i2cwrite((uint8_t)reg);
    i2cwrite((uint8_t)(value>>8));
    i2cwrite((uint8_t)(value & 0xFF));
    Wire.endTransmission();
}

/**************************************************************************/
/*!
@brief  Reads the 16-bit conversion register
In BOTH differential and single-ended modes, this value is a signed 16-bit integer (-32786 to 32767).
In single-ended mode, the value CAN drop below 0 due to noise etc. 
However, the ADC input should not be DRIVEN below 0V.
Therefore the effective range of the single ended mode in only 15 bits (0 to 32767)
*/
/**************************************************************************/
static int16_t readConversionRegister(uint8_t i2cAddress) {
    Wire.beginTransmission(i2cAddress);
    i2cwrite(ADS1x1x_REG_POINTER_CONVERT);
    Wire.endTransmission();
    Wire.requestFrom(i2cAddress, (uint8_t)2);
    return ((i2cread() << 8) | i2cread());  
}

/**************************************************************************/
/*!
@brief  Sets up the comparator to operate in basic mode, causing the
ALERT/RDY pin to assert (go from high to low) when the ADC
value exceeds the specified threshold.

This will also set the ADC in continuous conversion mode.
*/
/**************************************************************************/
static void _startComparator_SingleEnded(ADS1x1x * pParent, uint8_t channel, int16_t threshold)
{
    // Start with default values
    uint16_t config = ADS1x1x_DEFAULT_COMPARATOR_CONFIG;

    // Set PGA/voltage range
    config |= s_gainSettings[pParent->getGain()];

    // Set single-ended input channel
    config |= s_channels[channel];

    // Set the high threshold register
    // Shift 12-bit results left 4 bits for the ADS1x15
    writeRegister(pParent->getAddress(), ADS1x1x_REG_POINTER_HITHRESH, threshold << pParent->getBitShift());

    // Write config register to the ADC
    writeRegister(pParent->getAddress(), ADS1x1x_REG_POINTER_CONFIG, config);
}

/*
 * Class Functions 
 */

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
    m_fake = false;
    m_minFakeRead[0] = m_minFakeRead[1] = m_minFakeRead[2] = m_minFakeRead[3] = 0;
    m_maxFakeRead[0] = m_maxFakeRead[1] = m_maxFakeRead[2] = m_maxFakeRead[3] = 0;
}

ADS1013::ADS1013(uint8_t i2cAddress) : ADS1x1x(i2cAddress) {}
ADS1014::ADS1014(uint8_t i2cAddress) : ADS1x1x(i2cAddress) {}
ADS1015::ADS1015(uint8_t i2cAddress) : ADS1x1x(i2cAddress) {}
ADS1113::ADS1113(uint8_t i2cAddress) : ADS1x1x(i2cAddress) {}
ADS1114::ADS1114(uint8_t i2cAddress) : ADS1x1x(i2cAddress) {}
ADS1115::ADS1115(uint8_t i2cAddress) : ADS1x1x(i2cAddress) {}

/**************************************************************************/
/*!
@brief  Sets up the class to return fake ADC readings
*/
/**************************************************************************/
void ADS1x1x::fake(uint8_t ch, uint16_t minFakeRead, uint16_t maxFakeRead)
{
    m_fake = true;
    m_minFakeRead[ch] = minFakeRead;
    m_maxFakeRead[ch] = maxFakeRead;
}

/**************************************************************************/
/*!
@brief  Sets up the HW (reads coefficients values, etc.)
*/
/**************************************************************************/
void ADS1x1x::begin() {
    Wire.begin();
}

/**************************************************************************/
/*!
@brief  Sets the gain and input voltage range
*/
/**************************************************************************/
void ADS1x1x::setGain(ADS_GAIN gain)
{
    m_gain = gain;
}

/**************************************************************************/
/*!
@brief  Gets a gain and input voltage range
*/
/**************************************************************************/
ADS_GAIN ADS1x1x::getGain()
{
    return m_gain;
}

/**************************************************************************/
/*!
@brief  Gets the current i2c address
*/
/**************************************************************************/
uint8_t ADS1x1x::getAddress()
{
    return m_i2cAddress;
}


/**************************************************************************/
/*!
@brief  Gets a single-ended ADC reading from the specified channel
*/
/**************************************************************************/
int16_t ADS1x1x::readADC_SingleEnded(uint8_t channel)
{
    if (channel >= getMaxChannels())
    {
        return 0;
    }

    // Start with default values
    uint16_t config = ADS1x1x_DEFAULT_ADC_CONFIG;

    // Set PGA/voltage range
    config |= s_gainSettings[m_gain];

    // Set single-ended input channel
    config |= s_channels[channel];

    // Set 'start single-conversion' bit
    config |= ADS1x1x_REG_CONFIG_OS_SINGLE;

    // Write config register to the ADC
    writeRegister(m_i2cAddress, ADS1x1x_REG_POINTER_CONFIG, config);

    // Wait for the conversion to complete
    delay(getConversionTime());

    // Read the conversion results
    // Shift 12-bit results right 4 bits for the ADS1x15
    return readConversionRegister(m_i2cAddress) >> getBitShift();
}

/**************************************************************************/
/*! 
@brief  Reads the conversion results, measuring the voltage
difference between the P (AIN0) and N (AIN1) input.  Generates
a signed value since the difference can be either
positive or negative.
*/
/**************************************************************************/
int16_t ADS1x1x::readADC_Differential(uint8_t channel)
{
    return channel == 0 ? readADC_DifferentialWithConfig(ADS1x1x_REG_CONFIG_MUX_DIFF_0_1) : 0;
}

/**************************************************************************/
/*! 
@brief  Reads the conversion results, measuring the voltage
difference between the P (AIN2) and N (AIN3) input.  Generates
a signed value since the difference can be either
positive or negative.
*/
/**************************************************************************/
int16_t ADS1015::readADC_Differential(uint8_t channel)
{
    return channel < 2 ? readADC_DifferentialWithConfig(s_differentialChannels[channel]) : 0;
}

int16_t ADS1115::readADC_Differential(uint8_t channel)
{
    return channel < 2 ? readADC_DifferentialWithConfig(s_differentialChannels[channel]) : 0;
}


/**************************************************************************/
/*! 
@brief  Reads the conversion results, measuring the voltage
difference between two differential inputs.  Generates
a signed value since the difference can be either
positive or negative.
*/
/**************************************************************************/
int16_t ADS1x1x::readADC_DifferentialWithConfig(uint16_t differentialConfig)
{
    // Start with default values
    uint16_t config = ADS1x1x_DEFAULT_ADC_CONFIG;

    // Set PGA/voltage range
    config |= s_gainSettings[m_gain];

    // Set channels
    config |= differentialConfig;

    // Set 'start single-conversion' bit
    config |= ADS1x1x_REG_CONFIG_OS_SINGLE;

    // Write config register to the ADC
    writeRegister(m_i2cAddress, ADS1x1x_REG_POINTER_CONFIG, config);

    // Wait for the conversion to complete
    delay(getConversionTime());

    // Read the conversion results
    uint8_t bitShift = getBitShift();

    uint16_t res = readConversionRegister(m_i2cAddress) >> bitShift;
    if (bitShift == 0)
    {
        return (int16_t)res;
    }
    else
    {
        // Shift 12-bit results right 4 bits for the ADS1x15,
        // making sure we keep the sign bit intact
        if (res > 0x07FF)
        {
    // negative number - extend the sign to 16th bit
            res |= 0xF000;
        }
        return (int16_t)res;
    }
}

/**************************************************************************/
/*!
@brief  In order to clear the comparator, we need to read the
conversion results.  This function reads the last conversion
results without changing the config value.
*/
/**************************************************************************/
int16_t ADS1x1x::getLastConversionResults()
{
// Wait for the conversion to complete
    delay(getConversionTime());

// Read the conversion results
    uint8_t bitShift = getBitShift();
    uint16_t res = readConversionRegister(m_i2cAddress) >> bitShift;
    if (bitShift == 0)
    {
        return (int16_t)res;
    }
    else
    {
// Shift 12-bit results right 4 bits for the ADS1x15,
// making sure we keep the sign bit intact
        if (res > 0x07FF)
        {
// negative number - extend the sign to 16th bit
            res |= 0xF000;
        }
        return (int16_t)res;
    }
}

void ADS1x1x::startComparator_SingleEnded(uint8_t channel, int16_t threshold)
{
    if (hasComparator())
    {
        _startComparator_SingleEnded(this, channel, threshold);    
    }
    
}