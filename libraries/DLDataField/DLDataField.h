#ifndef _DATA_FIELD_H_
#define _DATA_FIELD_H_

enum field_type
{
    // Electrical values
    VOLTAGE,
    CURRENT,

    // Temperature values
    TEMPERATURE_C,
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
        DataField(FIELD_TYPE fieldType);
        ~DataField();
        FIELD_TYPE getType(void);
        
    protected:
        FIELD_TYPE m_fieldType;
        uint8_t m_index;
        uint8_t m_maxIndex;
};

template <typename T>
class NumericDataField : public DataField
{
    public:
        NumericDataField(FIELD_TYPE type, uint8_t N); // N is length of storage buffer
        ~NumericDataField();

        void storeData(T data);
        T getData(uint8_t index);
        void getDataAsString(char * buf, char const * const fmt, uint8_t index);

    private:
        T * m_data;
};

class StringDataField : public DataField
{
    public:
        // len is length of each string
        // N is number of strings to store
        StringDataField(FIELD_TYPE type, uint8_t len, uint8_t N);
        ~StringDataField();

        void storeData(char * data);
        char * getData(uint8_t index);
        void copy(char * buf, uint8_t index);
    private:
        char ** m_data;
        uint8_t m_maxLength;
};

#endif