#ifndef _DATA_FIELD_H_
#define _DATA_FIELD_H_

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

        void StoreData(void * data);
        void GetDataAsString(char * buf, char * fmt);
        float GetDataAsFloat(char * fmt);
};

#endif