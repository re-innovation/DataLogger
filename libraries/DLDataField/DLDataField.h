#ifndef _DATA_FIELD_H_
#define _DATA_FIELD_H_

#include "DLDataField.Types.h"

class DataField
{
    public:
        DataField(FIELD_TYPE fieldType, uint32_t length);
        ~DataField();
        FIELD_TYPE getType(void);    
        char const * getTypeString(void);

    protected:

        void incrementIndexes(void);
        uint32_t getRealReadIndex(uint32_t requestedIndex);

        FIELD_TYPE m_fieldType;
        bool m_full; // Set to true when buffer is first filled
        uint32_t m_index[2];
        uint32_t m_maxIndex;

};

template <typename T>
class NumericDataField : public DataField
{
    public:
        NumericDataField(FIELD_TYPE type, uint32_t N); // N is length of storage buffer
        ~NumericDataField();

        void storeData(T data);
        T getData(uint32_t index);
        void getDataAsString(char * buf, char const * const fmt, uint8_t index);

        bool isString(void) { return false; }
        bool isNumeric(void) { return true; }
        
    private:
        T * m_data;
        #ifdef TEST
        void printContents(void);
        #endif
};

class StringDataField : public DataField
{
    public:
        // len is length of each string
        // N is number of strings to store
        StringDataField(FIELD_TYPE type, uint8_t len, uint32_t N);
        ~StringDataField();

        void storeData(char const * data);
        char * getData(uint32_t index);
        void copy(char * buf, uint32_t index);

        bool isString(void) { return true; }
        bool isNumeric(void) { return false; }
        
    private:
        char ** m_data;
        uint8_t m_maxLength;
};

uint32_t DataField_writeHeadersToBuffer(
    char * buffer, StringDataField datafields[], uint8_t arrayLength, uint8_t bufferLength);

template <typename T>
uint32_t DataField_writeHeadersToBuffer(
    char * buffer, NumericDataField<T> ** datafields, uint8_t arrayLength, uint8_t bufferLength);

/* These functions are in-progress and don't really do the job they say they do quite right.
template <typename T>
uint32_t DataField_writeNumericDataToBuffer(
    char * buffer, NumericDataField<T> datafields[], char const * const format, uint8_t arrayLength, uint8_t bufferLength);

uint32_t DataField_writeStringDataToBuffer(
    char * buffer, StringDataField datafields[], uint8_t arrayLength, uint8_t bufferLength);
*/
#endif