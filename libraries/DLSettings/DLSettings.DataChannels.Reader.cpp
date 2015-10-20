/*
 * DLSettings.DataChannel.Reader.cpp
 *
 * Sets per-channel settings based on lines read from config file or similar
 *
 * Author: James Fowkes
 *
 * www.re-innovation.co.uk
 */

/*
 * Standard Library Includes
 */

#include <stdint.h>

/*
 * DataLogger Includes
 */

#include "DLUtility.Averager.h"
#include "DLLocalStorage.h"
#include "DLDataField.Types.h"
#include "DLDataField.h"
#include "DLDataField.Manager.h"
#include "DLSettings.Reader.Errors.h"
#include "DLSettings.DataChannels.h"

/*
 * Public Functions
 */

/*
 * Settings_readChannelsFromFile
 *
 * Read datachannel settings from the specified file
 * and configure the datafield manager
 *
 * pManager - pointer to the application datafield manager
 * pInterface - pointer to the application storage interface
 * filename - filename to read
 */
SETTINGS_READER_RESULT Settings_readChannelsFromFile(
    DataFieldManager * pManager, LocalStorageInterface * pInterface, char const * const filename)
{
    char lineBuffer[256];
    int lineCount = 1;
    SETTINGS_READER_RESULT err = ERR_READER_NONE;

    if (!pManager)
    {
        return noManagerError();
    }

    if (!pInterface)
    {
        return noInterfaceError();
    }

    if (!pInterface->fileExists(filename))
    {
        return noFile(filename);
    }
    
    uint8_t hndl = pInterface->openFile(filename, false);
    while (!pInterface->endOfFile(hndl))
    {
        // Read from file into lineBuffer and strip CRLF endings
        pInterface->readLine(hndl, lineBuffer, 256, true);
        err = Settings_parseDataChannelSetting(lineBuffer, lineCount++);
        if (err != ERR_READER_NONE) { break; } // Stop on any error encountered
    }

    pInterface->closeFile(hndl);

    if (err == ERR_READER_NONE)
    {
        pManager->setupAllValidChannels();
    }

    return err;
}
