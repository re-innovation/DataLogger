#ifndef _DATAFIELD_MANAGER_H_
#define _DATAFIELD_MANAGER_H_

#define MAX_FIELDS 32

class DataFieldManager
{
    public:
        DataFieldManager();
        count();
        bool addField(DataField * field);
        DataField * getField(uint8_t index);
        DataField ** getFields(void);
    private:
        DataField * m_fields[MAX_FIELDS];
        uint8_t m_count;
};

#endif
