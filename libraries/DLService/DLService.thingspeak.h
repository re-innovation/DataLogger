#ifndef _THINGSPEAK_H_
#define _THINGSPEAK_H_

#define _MAX_URL_LENGTH 50
#define _MAX_API_KEY_LENGTH 30

// Forward declarations of classes/structs
class FixedLengthAccumulator;

class Thingspeak : public ServiceInterface
{
    public:
        Thingspeak(char const * const url, char const * const key);
        ~Thingspeak();
    
        char *  getURL(void);

        uint16_t createPostAPICall(
            char * buffer, float * data, uint32_t * channels, uint8_t nFields, uint16_t maxSize);
        uint16_t createPostAPICall(
            char * buffer, float * data, uint32_t * channels, uint8_t nFields, uint16_t maxSize, char const * const time);

        void createBulkUploadCall(char * buffer, uint16_t maxSize, const char * csvData, const char * filename, uint8_t nFields);

    private:

        void putCSVUploadHeaders(FixedLengthAccumulator * accumulator, uint8_t nFields);
        
        static const char THINGSPEAK_UPDATE_PATH[];
        static const char THINGSPEAK_BULK_UPDATE_PATH[];
        static const char THINGSPEAK_MULTIPART_BOUNDARY[];
        char m_url[_MAX_URL_LENGTH];
        char m_key[_MAX_API_KEY_LENGTH];
};

#endif
