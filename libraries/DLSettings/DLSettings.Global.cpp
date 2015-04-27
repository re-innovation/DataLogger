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
#include <stdio.h>

/*
 * Local Includes
 */

#include "DLUtility.Averager.h"
#include "DLSettings.h"
#include "DLSettings.Global.h"
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
    
        int32_t get();
        bool isSet(void);
        void set(int32_t);
        void reset();
    private:
        int32_t m_setting;
        bool m_set;
};

StringSetting::StringSetting() {m_pSetting = NULL;}
StringSetting::~StringSetting() {}

IntSetting::IntSetting() { m_set = false; m_setting = 0;}
IntSetting::~IntSetting() {}

bool StringSetting::isSet() {return m_pSetting != NULL;}
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
void IntSetting::set(int32_t setting) { if (!m_set) { m_setting = setting; m_set = true;} }
int32_t IntSetting::get(void) { return m_setting; }

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

static uint8_t s_intSettingCount = 0;
static uint8_t s_stringSettingCount = 0;

void Settings_InitGlobal(void)
{
    uint8_t i;
    for (i = 0; i < INT_SETTINGS_COUNT; ++i)
    {
        Settings_resetInt((INTSETTING)i);
    }


    // Searching for ints didn't work, try to find string setting
    for (i = 0; i < STRING_SETTINGS_COUNT; ++i)
    {
        Settings_resetString((STRINGSETTING)i);
    }

    s_stringSettingCount = 0;
    s_intSettingCount = 0;
}

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
        s_stringSettingCount++;
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

int32_t Settings_getInt(INTSETTING setting)
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

void Settings_setInt(INTSETTING setting, int32_t set)
{
    if (setting < INT_SETTINGS_COUNT)
    {
        s_ints[setting].set(set);
        s_intSettingCount++;
    }  
}

uint8_t Settings_getIntCount(void)
{
    return s_intSettingCount;
}

uint8_t Settings_getStringCount(void)
{
    return s_stringSettingCount;
}

uint8_t Settings_getCount(void)
{
    return s_intSettingCount + s_stringSettingCount;
}

void Settings_echoAllSet(PRINTFN printfn)
{
    // Echo out integer settings

    int i;

    char intBuffer[10];

    printfn("Successfully read ");
    sprintf(intBuffer, "%d", Settings_getCount());
    printfn(intBuffer);
    printfn(" settings\r\n");
    
    if ((Settings_getIntCount()))
    {
        printfn("Integer Settings:\r\n");
    
        for (i = 0; i < INT_SETTINGS_COUNT; i++)
        {
            if (Settings_intIsSet((INTSETTING)i))
            {
                sprintf(intBuffer, "%d", Settings_getInt((INTSETTING)i));
                printfn(Settings_getIntName((INTSETTING)i));
                printfn(": ");
                printfn(intBuffer);
                printfn("\r\n");
            }
        }
    }

    // Echo out string settings
    if ((Settings_getStringCount()))
    {
        printfn("String Settings:\r\n");
        for (i = 0; i < STRING_SETTINGS_COUNT; i++)
        {
            if (Settings_stringIsSet((STRINGSETTING)i))
            {
                printfn(Settings_getStringName((STRINGSETTING)i));
                printfn(": ");
                printfn(Settings_getString((STRINGSETTING)i));
                printfn("\r\n");
            }
        }   
    }
}
