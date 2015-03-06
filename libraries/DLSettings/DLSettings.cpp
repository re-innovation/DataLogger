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
    
        char * get();
        bool isSet(void);
        void set(char const * const);
        
    private:
        char * m_pSetting;
};

class IntSetting
{
    public:
        IntSetting(void);
        
        ~IntSetting();
    
        int get();
        bool isSet(void);
        void set(int);
    
    private:
        int m_setting;
        bool m_set;
};

StringSetting::StringSetting() {m_pSetting = NULL;}
StringSetting::~StringSetting() {}


IntSetting::IntSetting() { m_set = false; m_setting = 0;}
IntSetting::~IntSetting() {}


bool StringSetting::isSet() {return m_pSetting == NULL;}
void StringSetting::set(char const * const pSetting)
{
    if (!m_pSetting)
    {
        uint8_t length = strlen(pSetting);
        m_pSetting = new char[length+1];
        strncpy(m_pSetting, pSetting, length);
		m_pSetting[length] = '\0';
    }
}
char * StringSetting::get(void) { return m_pSetting; }

bool IntSetting::isSet() {return m_set;}
void IntSetting::set(int setting) { if (!m_set) { m_setting = setting; m_set = true;} }
int IntSetting::get(void) { return m_setting; }

/////////////////////////////////////////////////////////

/*
 * Each setting for the application is either a StringSetting or IntSetting
 */

static StringSetting s_strings[STRING_SETTINGS_COUNT];
static IntSetting s_ints[INT_SETTINGS_COUNT];

bool Settings_stringIsSet(STRINGSETTING setting)
{
    if (setting < STRING_SETTINGS_COUNT)
    {
        return s_strings[setting].isSet();
    }
    else
    {
        return false;
    }
}

char * Settings_getString(STRINGSETTING setting)
{
    if (setting < STRING_SETTINGS_COUNT)
    {
        return s_strings[setting].get();
    }
    else
    {
        return NULL;
    }
}

void Settings_setString(STRINGSETTING setting, char const * const pSet)
{
    if (setting < STRING_SETTINGS_COUNT)
    {
        s_strings[setting].set(pSet);
    }
}

bool Settings_intisSet(INTSETTING setting)
{
    if (setting < INT_SETTINGS_COUNT)
    {
        return s_ints[setting].isSet();
    }
    else
    {
        return false;
    } 
}

int Settings_getInt(INTSETTING setting)
{
    if (setting < INT_SETTINGS_COUNT)
    {
        return s_ints[setting].get();
    }
    else
    {
        return 0;
    }    
}

void Settings_setInt(INTSETTING setting, int set)
{
    if (setting < INT_SETTINGS_COUNT)
    {
        s_ints[setting].set(set);
    }  
}
