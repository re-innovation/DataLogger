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
 * Standard Library Includes
 */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
 * Local Includes
 */

#include "DLSettings.h"

/*
 * Define two small classes to hold write-once read-many integer and string settings
 */ 
class StringSetting
{
    public:
        StringSetting();
        ~StringSetting();
    
        char * Get();
        void Set(char const * const);
        
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
        bool m_set;
};

StringSetting::StringSetting() {m_pSetting = NULL;}
StringSetting::~StringSetting() {}

IntSetting::IntSetting() { m_set = false; m_setting = 0;}
IntSetting::~IntSetting() {}

void StringSetting::Set(char const * const pSetting)
{
    if (!m_pSetting)
    {
        uint8_t length = strlen(pSetting);
        m_pSetting = new char[length];
        strncpy(m_pSetting, pSetting, length);
    }
}

char * StringSetting::Get(void) { return m_pSetting; }

void IntSetting::Set(int setting) { if (!m_set) { m_setting = setting; m_set = true;} }
int IntSetting::Get(void) { return m_setting; }

/////////////////////////////////////////////////////////

/*
 * Each setting for the application is either a StringSetting or IntSetting
 */

static StringSetting s_strings[STRING_SETTINGS_COUNT];
static IntSetting s_ints[INT_SETTINGS_COUNT];

char * Settings_GetString(STRINGSETTING setting)
{
    if (setting < STRING_SETTINGS_COUNT)
    {
        return s_strings[setting].Get();
    }
    else
    {
        return NULL;
    }
}

void Settings_SetString(STRINGSETTING setting, char const * const pSet)
{
    if (setting < STRING_SETTINGS_COUNT)
    {
        s_strings[setting].Set(pSet);
    }
}

int Settings_GetInt(INTSETTING setting)
{
    if (setting < INT_SETTINGS_COUNT)
    {
        return s_ints[setting].Get();
    }
    else
    {
        return 0;
    }    
}

void Settings_SetInt(INTSETTING setting, int set)
{
    if (setting < INT_SETTINGS_COUNT)
    {
        s_ints[setting].Set(set);
    }  
}
