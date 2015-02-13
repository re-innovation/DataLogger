#ifndef _SERVICE_IF_H_
#define _SERVICE_IF_H_

enum service
{
    SERVICE_THINGSPEAK
};
typedef enum service SERVICE;

class ServiceInterface
{
    public:
        virtual void Set(uint8_t fieldIndex, DataField * data) = 0;

        virtual uint16_t CreateGetAPICall(char * buffer) = 0;
        virtual uint16_t CreateGetAPICall(char * buffer, char const * const time) = 0;
        
        virtual uint16_t CreateGetAPIParamsString(char * buffer) = 0;
        virtual uint16_t CreateGetAPIParamsString(char * buffer, char const * const time) = 0;
        
};

ServiceInterface * GetService(SERVICE service);

#endif

