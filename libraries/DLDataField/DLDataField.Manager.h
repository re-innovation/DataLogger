#ifndef _DATAFIELD_MANAGER_H_
#define _DATAFIELD_MANAGER_H_

#define MAX_FIELDS 32

class DataFieldManager
{
    public:
        DataFieldManager(uint32_t dataSize, uint32_t averagerSize);
        uint8_t count();
        bool addField(NumericDataField * field);
        bool addField(StringDataField * field);
        DataField * getField(uint8_t index);
        DataField ** getFields(void);

        uint32_t writeHeadersToBuffer(char * buffer, uint8_t bufferLength);
        
        void setupAllValidChannels(void);

        bool hasData(void);
        
    private:
        DataField * m_fields[MAX_FIELDS];
        uint8_t m_count;
        uint32_t m_dataSize;
        uint32_t m_averagerSize;
};

#endif
