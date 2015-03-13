#ifndef _DATA_FIELD_H_
#define _DATA_FIELD_H_

#define MAX_DATAFIELD_STRING_SIZE (12)

enum field_type
{
    // Generic types that have no particular context
    GENERIC_FLOAT,
    GENERIC_INT,
    GENERIC_STRING,

    // Electrical values
    VOLTAGE,
    CURRENT,

    // Environmental values
    TEMPERATURE_C,
    TEMPERATURE_K,
    IRRADIANCE_WpM2
};
typedef enum field_type FIELD_TYPE;

class DataField
{
    public:
        DataField(FIELD_TYPE type);
        ~DataField();

        template <typename T>
        void storeData(T data);
        void storeData(char * str);
        void storeData(char const * const str);
        void getDataAsString(char * buf, char const * const fmt);
        float getDataAsFloat(void);
        FIELD_TYPE getType(void);
    private:
        FIELD_TYPE m_fieldType;
        float m_data;
        char m_stringData[MAX_DATAFIELD_STRING_SIZE+1];
};

#endif