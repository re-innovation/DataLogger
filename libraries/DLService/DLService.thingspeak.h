#ifndef _THINGSPEAK_H_
#define _THINGSPEAK_H_

#define _MAX_URL_LENGTH 50
#define _MAX_API_KEY_LENGTH 30
#define _MAX_FIELDS 8

class Thingspeak : public ServiceInterface
{
    public:
        Thingspeak(char const * const url, char const * const key);
        ~Thingspeak();
    
        void setField(uint8_t fieldIndex, DataField * data);
        char *  getURL(void);

        uint16_t createGetAPICall(char * buffer, uint16_t maxSize);
        uint16_t createGetAPICall(char * buffer, uint16_t maxSize, char const * const time);

        static const char THINGSPEAK_GET_PATH[];
        
    private:
        char m_url[_MAX_API_KEY_LENGTH];
        char m_key[_MAX_API_KEY_LENGTH];
        float m_data[_MAX_FIELDS];
};

#endif
