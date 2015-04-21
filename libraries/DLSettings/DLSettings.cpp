/*
 * DLSettings.cpp
 * 
 * Setting interface for the application
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
 
void Settings_Init(void)
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

    Settings_InitDataChannels();
    Settings_InitReader();
}
