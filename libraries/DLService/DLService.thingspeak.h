#ifndef _THINGSPEAK_H_
#define _THINGSPEAK_H_

#define _MAX_API_KEY_LENGTH 30
#define _MAX_FIELDS 8

class Thingspeak : public ServiceInterface
{
    public:
        Thingspeak(char const * const key);
        ~Thingspeak();
    
        void SetField(uint8_t fieldIndex, DataField * data);
        
        uint16_t CreateGetAPICall(char * buffer);
        uint16_t CreateGetAPICall(char * buffer, char const * const time);
        
        uint16_t CreateGetAPIParamsString(char * buffer);
        uint16_t CreateGetAPIParamsString(char * buffer, char const * const time);
        
        static const char THINGSPEAK_URL[];
        static const char THINGSPEAK_GET_PATH[];
        
    private:
        char m_key[_MAX_API_KEY_LENGTH];
        float m_data[_MAX_FIELDS];
};

#endif
