/*
 * settings.cpp
 * 
 * Holds settings for the application
 *
 * Author: James Fowkes
 *
 * www.re-innovation.co.uk
 */

/*
 * Define two small classes to hold write-once read-many integer and string settings
 */ 
class StringSetting
{
    public:
        StringSetting();
        ~StringSetting();
    
        char * Get();
        void Set(char *);
        
    private:
        char * m_pSetting;
};

class IntSetting
{
    public:
        IntSetting(void);
        
        ~IntSetting();
    
        int Get();
        void Set(int);
    
    private:
        int m_setting;
        bool set
};

StringSetting::StringSetting() {m_pSetting = NULL;}
StringSetting::~StringSetting() {}

IntSetting::IntSetting() { set = false;}
IntSetting::~IntSetting() {}

StringSetting::Set(char * pSetting} { if (!m_pSetting) { m_pSetting = pSetting;}
char * StringSetting::Get(void} { return m_pSetting; }

IntSetting::Set(int setting} { if (!set) { m_setting = setting; set = true;}
char * IntSetting::Get(void} { return m_setting; }

/////////////////////////////////////////////////////////

/*
 * Each setting for the application is either a StringSetting or IntSetting
 */

static StringSetting s_strings[STRING_SETTINGS_COUNT];
static IntSetting s_ints[INT_SETTINGS_COUNT];

char * GetString(STRINGSETTING setting)
{
    if (setting < STRING_SETTINGS_COUNT)
    {
        return s_strings[STRING_SETTINGS_COUNT].Get();
    }
    else
    {
        return NULL;
    }
}

void SetString(STRINGSETTING setting, char * pSet);
{
    if (setting < STRING_SETTINGS_COUNT)
    {
        s_strings[STRING_SETTINGS_COUNT].Set(pSet);
    }
}

int GetInt(INTSETTING setting)
{
    if (setting < INT_SETTINGS_COUNT)
    {
        return s_ints[INT_SETTINGS_COUNT].Get();
    }
    else
    {
        return NULL;
    }    
}

void SetInt(INTSETTING setting, int set)
{
    if (setting < INT_SETTINGS_COUNT)
    {
        s_ints[INT_SETTINGS_COUNT].Set(set);
    }  
}
