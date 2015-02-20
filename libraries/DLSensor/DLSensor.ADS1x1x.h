#ifndef _DLSENSOR_ADS1x1x_H_
#define _DLSENSOR_ADS1x1x_H_

/*=========================================================================
    I2C ADDRESS/BITS
    -----------------------------------------------------------------------*/
    #define DEFAULT_ADS1x1x_ADDRESS                 (0x48)    // 1001 000 (ADDR = GND)
/*=========================================================================*/

enum ads_gain
{
    GAIN_TWOTHIRDS,
    GAIN_ONE,
    GAIN_TWO,
    GAIN_FOUR,
    GAIN_EIGHT,
    GAIN_SIXTEEN
};
typedef enum ads_gain ADS_GAIN;

/* Generic class for all ADS1x1x ADCs */
class ADS1x1x
{
    protected:
        // Instance-specific properties
        uint8_t     m_i2cAddress;
        ADS_GAIN    m_gain;

        int16_t readADC_DifferentialWithConfig(uint16_t);

    public:
        ADS1x1x(uint8_t i2cAddress = DEFAULT_ADS1x1x_ADDRESS);
        void      begin(void);
        uint16_t  readADC_SingleEnded(uint8_t channel);
        int16_t   readADC_Differential(uint8_t channel);
        int16_t   getLastConversionResults();
        void      setGain(ADS_GAIN gain);
        ADS_GAIN  getGain(void);
        uint8_t   getAddress(void);
        void      startComparator_SingleEnded(uint8_t channel, int16_t threshold);

        virtual uint8_t getMaxChannels(void) = 0;
        virtual uint8_t  getConversionTime(void) = 0;
        virtual uint8_t  getResolution(void) = 0;
        virtual bool hasComparator(void) = 0;
        virtual uint8_t getBitShift(void) = 0;
};

class ADS1013 : public ADS1x1x
{
    public:
        ADS1013(uint8_t i2cAddress = DEFAULT_ADS1x1x_ADDRESS);
        bool hasComparator(void) { return false; }
        uint8_t getMaxChannels(void) { return 2; }
        uint8_t  getResolution(void) { return 12; };
        uint8_t getBitShift(void) { return 4; }
        uint8_t  getConversionTime(void) { return 1; }
};

class ADS1014 : public ADS1x1x
{
    public:
        ADS1014(uint8_t i2cAddress = DEFAULT_ADS1x1x_ADDRESS);

        bool hasComparator(void) { return true; }
        uint8_t getMaxChannels(void) { return 2; }
        uint8_t  getResolution(void) { return 12; };
        uint8_t getBitShift(void) { return 4; }
        uint8_t  getConversionTime(void) { return 1; }
        

};

class ADS1015 : public ADS1x1x
{
    public:
        ADS1015(uint8_t i2cAddress = DEFAULT_ADS1x1x_ADDRESS);
        int16_t readADC_Differential(uint8_t channel);

        bool hasComparator(void) { return true; }
        uint8_t getMaxChannels(void) { return 4; }
        uint8_t  getResolution(void) { return 12; };
        uint8_t getBitShift(void) { return 4; }
        uint8_t  getConversionTime(void) { return 1; }

};

class ADS1113 : public ADS1x1x
{
    public:
        ADS1113(uint8_t i2cAddress = DEFAULT_ADS1x1x_ADDRESS);
        bool hasComparator(void) { return false; }
        uint8_t getMaxChannels(void) { return 2; }
        uint8_t  getResolution(void) { return 16; };
        uint8_t getBitShift(void) { return 4; }
        uint8_t  getConversionTime(void) { return 8; }
};
class ADS1114 : public ADS1x1x
{
    public:
        ADS1114(uint8_t i2cAddress = DEFAULT_ADS1x1x_ADDRESS);

        bool hasComparator(void) { return true; }
        uint8_t getMaxChannels(void) { return 2; }
        uint8_t  getResolution(void) { return 16; };
        uint8_t getBitShift(void) { return 4; }
        uint8_t  getConversionTime(void) { return 8; }
};
class ADS1115 : public ADS1x1x
{
    public:
        ADS1115(uint8_t i2cAddress = DEFAULT_ADS1x1x_ADDRESS);
        int16_t readADC_Differential(uint8_t channel);

        bool hasComparator(void) { return true; }
        uint8_t getMaxChannels(void) { return 4; }
        uint8_t  getResolution(void) { return 16; };
        uint8_t getBitShift(void) { return 4; }
        uint8_t  getConversionTime(void) { return 8; }

};
#endif