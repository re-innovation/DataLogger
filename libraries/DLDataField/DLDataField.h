#ifndef _DATA_FIELD_H_
#define _DATA_FIELD_H_

enum field_type
{
    // Electrical values
    VOLTAGE,
    CURRENT,

    // Temperature values
    TEMPERATURE_C,
    TEMPERATURE_F,
    TEMPERATURE_K,
    
    // Solar values
    IRRADIANCE_WpM2,

    // Wind values
    CARDINAL_DIRECTION, //N, NE, E, etc.
    DEGREES_DIRECTION

};
typedef enum field_type FIELD_TYPE;

class DataField
{
    public:
        DataField(FIELD_TYPE fieldType, uint8_t length);
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
        NumericDataField(FIELD_TYPE type, uint8_t N); // N is length of storage buffer
        ~NumericDataField();

        void storeData(T data);
        T getData(uint32_t index);
        void getDataAsString(char * buf, char const * const fmt, uint8_t index);

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
        StringDataField(FIELD_TYPE type, uint8_t len, uint8_t N);
        ~StringDataField();

        void storeData(char * data);
        char * getData(uint32_t index);
        void copy(char * buf, uint32_t index);
    private:
        char ** m_data;
        uint8_t m_maxLength;
};

#endif