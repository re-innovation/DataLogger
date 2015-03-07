#ifndef _SERVICE_IF_H_
#define _SERVICE_IF_H_

enum service
{
    SERVICE_THINGSPEAK
};
typedef enum service SERVICE;

/*
 * Forward declarations of required classes
 */

class DataField;

/*
 * Service Interface class
 * - Defines a standard interface to push data to internet based data logging services
 */
class ServiceInterface
{
    public:
        virtual void setField(uint8_t fieldIndex, DataField * pDataField) = 0;
        virtual char *  getURL(void) = 0;
        
        virtual uint16_t createPostAPICall(char * buffer, uint16_t maxSize) = 0;
        virtual uint16_t createPostAPICall(char * buffer, uint16_t maxSize, char const * const time) = 0;
        
        virtual void createBulkUploadCall(char * buffer, uint16_t maxSize, const char * csvData, const char * filename) = 0;
};

ServiceInterface * Service_GetService(SERVICE service);

#endif

