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
#include "DLDataField.Types.h"
#include "DLDataField.h"
#include "DLDataField.Manager.h"
#include "DLLocalStorage.h"
#include "DLSettings.h"
#include "DLSettings.Global.h"
#include "DLSettings.Reader.h"
#include "DLSettings.DataChannels.h"

void Settings_Init(void)
{
    Settings_InitGlobal();
    Settings_InitDataChannels();
    Settings_InitReader();
}
