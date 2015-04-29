#ifndef _DATA_FIELD_H_
#define _DATA_FIELD_H_

// A datafield should return this value if data is requested when none exists
#define DATAFIELD_NO_DATA_VALUE (float)(0xFFFFFFFF)

class DataField
{
    public:
        DataField(FIELD_TYPE fieldType, uint32_t channelNumber);
        ~DataField();

        void setSize(uint32_t length);
        FIELD_TYPE getType(void);    
        char const * getTypeString(void);

        uint32_t length(void);
        bool hasData(void);
        void removeOldest(void);

        uint32_t getChannelNumber(void);

    protected:

        //void incrementIndexes(void);
        void pop(void);
        void prePush(void);
        void postPush(void);
        bool full(void);

        uint32_t getTailIndex(void);

        //uint32_t getRealReadIndex(uint32_t requestedIndex);

        uint32_t getWriteIndex(void);
        
        FIELD_TYPE m_fieldType;
        uint32_t m_index[2];
        uint32_t m_maxIndex;
        uint32_t m_channelNumber;
        Averager<int32_t> * m_averager;
};

class NumericDataField : public DataField
{
    public:
        /* type: VOLTAGE, CURRENT etc.
         * fieldData: Pointer to VOLTAGECHANNEL, CURRENTCHANNEL to match field type
         */
        NumericDataField(FIELD_TYPE type, void * fieldData, uint32_t channelNumber);
        ~NumericDataField();

        void setDataSizes(uint32_t N, uint32_t averagerN);

        void storeData(int32_t data);

        float getRawData(bool alsoRemove);
        float getConvData(bool alsoRemove);
        void getRawDataAsString(char * buf, char const * const fmt, bool alsoRemove);
        void getConvDataAsString(char * buf, char const * const fmt, bool alsoRemove);

        bool isString(void) { return false; }
        bool isNumeric(void) { return true; }

        void getConfigString(char * buffer);
        
    private:
        float * m_data;
        void * m_conversionData;
        #ifdef TEST
        void printContents(void);
        #endif
};

class StringDataField : public DataField
{
    public:
        // len is length of each string
        // N is number of strings to store
        StringDataField(FIELD_TYPE type, uint8_t len, uint32_t N, uint32_t channelNumber);
        ~StringDataField();

        void storeData(char const * data);
        char * getData(bool alsoRemove);
        void copy(char * buf, bool alsoRemove);

        bool isString(void) { return true; }
        bool isNumeric(void) { return false; }
        
    private:
        char ** m_data;
        uint8_t m_maxLength;
};

/* These functions are in-progress and don't really do the job they say they do quite right.
uint32_t DataField_writeNumericDataToBuffer(
    char * buffer, NumericDataField datafields[], char const * const format, uint8_t arrayLength, uint8_t bufferLength);

uint32_t DataField_writeStringDataToBuffer(
    char * buffer, StringDataField datafields[], uint8_t arrayLength, uint8_t bufferLength);
*/

// Conversion functions provided by DLDataField.Conversion.cpp
float CONV_VoltsFromRaw(float raw, VOLTAGECHANNEL * conversionData);
float CONV_AmpsFromRaw(float raw, CURRENTCHANNEL * conversionData);
float CONV_CelsiusFromRawThermistor(float raw, THERMISTORCHANNEL * conversionData);

#endif