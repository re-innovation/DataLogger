/*
 * LinkItONE.Readline.Example.cpp
 *
 * Example of using readLineWithReadFunction to read lines from
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
    uint8_t hndl = s_sdCard->openFile("FileWithCRLFEndings.txt", false);
    while (!s_sdCard->endOfFile(hndl))
    {
    	s_sdCard->readLine(hndl, lineBuffer, 50, true);
    	Serial.print(lineBuffer);
    }
    s_sdCard->closeFile(hndl);
    
    Serial.println("Reading from file FileWithLFEndings.txt:");
    
    hndl = s_sdCard->openFile("FileWithLFEndings.txt", false);
    while (!s_sdCard->endOfFile(hndl))
    {
    	s_sdCard->readLine(hndl, lineBuffer, 50, true);
    	Serial.print(lineBuffer);
    }
    s_sdCard->closeFile(hndl);
}

void loop()
{

}
