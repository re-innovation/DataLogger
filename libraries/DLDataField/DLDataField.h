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
        void StoreData(T data);
        void StoreData(char * str);
        void StoreData(char const * const str);
        void GetDataAsString(char * buf, char const * const fmt);
        float GetDataAsFloat(void);
        FIELD_TYPE GetType(void);
    private:
        FIELD_TYPE m_fieldType;
        float m_data;
        char m_stringData[MAX_DATAFIELD_STRING_SIZE+1];
};

#endif