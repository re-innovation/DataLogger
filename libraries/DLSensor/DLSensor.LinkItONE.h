#ifndef _DLSENSOR_LINKITONE_H_
#define _DLSENSOR_LINKITONE_H_

class LinkItONEADC
{
    public:
        LinkItONEADC(uint8_t channel);
        uint16_t read(void);

    private:
        uint8_t m_channel;
    
};

#endif