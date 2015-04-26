#ifndef _DATA_FIELD_H_
#define _DATA_FIELD_H_

/* Each datafield has an associated datatype */
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
    DEGREES_DIRECTION,

    INVALID_TYPE
};
typedef enum field_type FIELD_TYPE;

/* Each FIELD_TYPE has a data structure associated with it 
 * in order to perform conversions from raw values to units.
  In addition, the number of settings is #defined so that the
  settings module can figure out if everything is set */
struct voltagechannel
{
    float mvPerBit;
    float R1;
    float R2;
};
typedef struct voltagechannel VOLTAGECHANNEL;

struct currentchannel
{
    float mvPerBit;
    float offset;
    float mvPerAmp;
};
typedef struct currentchannel CURRENTCHANNEL;

class DataField
{
    public:
        DataField(FIELD_TYPE fieldType);
        ~DataField();

        void setSize(uint32_t length);
        FIELD_TYPE getType(void);    
        char const * getTypeString(void);

    protected:

        void incrementIndexes(void);
        uint32_t getRealReadIndex(uint32_t requestedIndex);
        uint32_t getWriteIndex(void);
        
        FIELD_TYPE m_fieldType;
        bool m_full; // Set to true when buffer is first filled
        uint32_t m_index[2];
        uint32_t m_maxIndex;

        Averager<int32_t> * m_averager;
};

class NumericDataField : public DataField
{
    public:
        /* type: VOLTAGE, CURRENT etc.
         * fieldData: Pointer to VOLTAGECHANNEL, CURRENTCHANNEL to match field type
         */
        NumericDataField(FIELD_TYPE type, void * fieldData);
        ~NumericDataField();

        void setDataSizes(uint32_t N, uint32_t averagerN);

        void storeData(int32_t data);

        float getRawData(uint32_t index);
        float getConvData(uint32_t index);
        void getDataAsString(char * buf, char const * const fmt, uint8_t index);

        bool isString(void) { return false; }
        bool isNumeric(void) { return true; }
        
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

/* These functions are in-progress and don't really do the job they say they do quite right.
uint32_t DataField_writeNumericDataToBuffer(
    char * buffer, NumericDataField datafields[], char const * const format, uint8_t arrayLength, uint8_t bufferLength);

uint32_t DataField_writeStringDataToBuffer(
    char * buffer, StringDataField datafields[], uint8_t arrayLength, uint8_t bufferLength);
*/

// Conversion functions provided by DLDataField.Conversion.cpp
float CONV_VoltsFromRaw(float raw, VOLTAGECHANNEL * conversionData);
float CONV_AmpsFromRaw(float raw, CURRENTCHANNEL * conversionData);

#endif