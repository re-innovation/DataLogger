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

#include "DLUtility.Averager.h"
#include "DLDataField.h"
#include "DLDataField.Manager.h"
#include "DLLocalStorage.h"
#include "DLSettings.h"
 
void Settings_Init(void)
{
    Settings_InitGlobal();
    Settings_InitDataChannels();
    Settings_InitReader();
}
