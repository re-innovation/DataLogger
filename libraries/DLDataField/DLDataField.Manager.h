#ifndef _DATAFIELD_MANAGER_H_
#define _DATAFIELD_MANAGER_H_

#define MAX_FIELDS 32

class DataFieldManager
{
    public:
        DataFieldManager(uint32_t dataSize, uint32_t averagerSize);
        uint8_t fieldCount();
        bool addField(NumericDataField * field);
        bool addField(StringDataField * field);
        DataField * getField(uint8_t index);
        DataField * getChannel(uint8_t index);
        DataField ** getFields(void);

        void storeDataArray(int32_t * data);
        void getConvDataArray(float * buffer, bool alsoRemove);
        uint32_t writeHeadersToBuffer(char * buffer, uint8_t bufferLength);
        
        void setupAllValidChannels(void);
        uint32_t * getChannelNumbers(void);
        bool hasData(void);
        uint32_t count(void);

    private:
        DataField * m_fields[MAX_FIELDS];
        uint8_t m_fieldCount;
        uint32_t m_count;
        uint32_t m_dataSize;
        uint32_t m_averagerSize;
        uint32_t m_channelNumbers[MAX_FIELDS];
};

#endif
