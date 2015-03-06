/*
 * LinkITONE.Readline.h
 *
 * Example of using ReadLineWithReadFunction to read lines from
 * files on an SD card in the LinkIt ONE board.
 *
 */

/*
 * Standard Library Includes
 */
#include <stdint.h>

/*
 * LinkIt One Includes
 */

#include <LSD.h>
#include <LStorage.h>

/*
 * DataLogger Includes
 */

#include "DLUtility.h"
#include "DLLocalStorage.h"

static LocalStorageInterface * s_sdCard;

void setup()
{
    // setup Serial port
    Serial.begin(115200);

    delay(10000);

    s_sdCard = LocalStorage_GetLocalStorageInterface(LINKITONE_SD_CARD);
	
	Serial.println("Reading from file FileWithCRLFEndings.txt:");
    char lineBuffer[50];
    uint8_t hndl = s_sdCard->OpenFile("FileWithCRLFEndings.txt", false);
    while (!s_sdCard->EndOfFile(hndl))
    {
    	s_sdCard->ReadLine(hndl, lineBuffer, 50);
    	Serial.print(lineBuffer);
    }
    s_sdCard->CloseFile(hndl);
    
    Serial.println("Reading from file FileWithLFEndings.txt:");
    
    hndl = s_sdCard->OpenFile("FileWithLFEndings.txt", false);
    while (!s_sdCard->EndOfFile(hndl))
    {
    	s_sdCard->ReadLine(hndl, lineBuffer, 50);
    	Serial.print(lineBuffer);
    }
    s_sdCard->CloseFile(hndl);
}

void loop()
{

}
