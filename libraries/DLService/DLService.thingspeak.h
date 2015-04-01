#ifndef _THINGSPEAK_H_
#define _THINGSPEAK_H_

#define _MAX_URL_LENGTH 50
#define _MAX_API_KEY_LENGTH 30

#ifndef _MAX_FIELDS // Might be defined by test suite
#define _MAX_FIELDS 8
#endif

class FixedLengthAccumulator;

class Thingspeak : public ServiceInterface
{
    public:
        Thingspeak(char const * const url, char const * const key);
        ~Thingspeak();
    
        void setField(uint8_t fieldIndex, float data);
        char *  getURL(void);

        uint16_t createPostAPICall(char * buffer, uint16_t maxSize);
        uint16_t createPostAPICall(char * buffer, uint16_t maxSize, char const * const time);

        void createBulkUploadCall(char * buffer, uint16_t maxSize, const char * csvData, const char * filename);
        
    private:

        void putCSVUploadHeaders(FixedLengthAccumulator * accumulator);
        
        static const char THINGSPEAK_UPDATE_PATH[];
        static const char THINGSPEAK_BULK_UPDATE_PATH[];
        static const char THINGSPEAK_MULTIPART_BOUNDARY[];
        char m_url[_MAX_URL_LENGTH];
        char m_key[_MAX_API_KEY_LENGTH];
        float m_data[_MAX_FIELDS];
};

#endif
