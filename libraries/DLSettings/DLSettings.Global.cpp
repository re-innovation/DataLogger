/*
 * DLSettings.Global.cpp
 * 
 * Holds global settings for the application
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
#include "DLUtility.h"

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
        void reset();
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
        void reset();
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
        strncpy_safe(m_pSetting, pSetting, length+1);
    }
}
char * StringSetting::get(void) { return m_pSetting; }

bool IntSetting::isSet() {return m_set;}
void IntSetting::set(int setting) { if (!m_set) { m_setting = setting; m_set = true;} }
int IntSetting::get(void) { return m_setting; }

#ifdef TEST
void StringSetting::reset(void) { m_pSetting = NULL; }
void IntSetting::reset(void) { m_set = false; }
#else
// Reset functions do nothing outside of testing context
void StringSetting::reset(void) { }
void IntSetting::reset(void) { }
#endif

/////////////////////////////////////////////////////////

/*
 * Each setting for the application is either a StringSetting or IntSetting
 */

/* Generate string strings */
static const char * s_stringSettingNames[] = {
    FOREACH_STRINGSET(GENERATE_STRING)  
};

/* Generate int strings */
static const char * s_intSettingNames[] = {
    FOREACH_INTSET(GENERATE_STRING)  
};

static StringSetting s_strings[STRING_SETTINGS_COUNT];
static IntSetting s_ints[INT_SETTINGS_COUNT];

void Settings_resetInt(INTSETTING setting)
{
    s_ints[setting].reset();
}

void Settings_resetString(STRINGSETTING setting)
{
    s_strings[setting].reset();
}

char const * Settings_getStringName(STRINGSETTING setting)
{
    if (setting < STRING_SETTINGS_COUNT)
    {
        return s_stringSettingNames[setting];
    }
    else
    {
        return NULL;
    }
}

char const * Settings_getIntName(INTSETTING setting)
{
    if (setting < INT_SETTINGS_COUNT)
    {
        return s_intSettingNames[setting];
    }
    else
    {
        return NULL;
    }
}

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

bool Settings_intIsSet(INTSETTING setting)
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
