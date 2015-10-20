/*
DLSettings.Reader.example.cpp

A sample command-line application showing usage of the settings file reader

Reads from "datalogger.settings" and echoes those settings back to the command line

*/

#include <iostream>
#include <fstream>
#include <string>

#include "DLUtility.Averager.h"
#include "DLDataField.h"
#include "DLDataField.Manager.h"
#include "DLLocalStorage.h"
#include "DLSettings.h"
#include "DLSettings.Reader.h"

static void localPrintFn(char const * const toPrint)
{
    std::cout << toPrint;
}

int main(int argc, char * argv[])
{
    (void)argc;
    (void)argv;

    std::string line;

    Settings_Init();

    std::cout << "Reading using Settings_readFromString one line at a time." << std::endl;
  	std::ifstream settingsFile("datalogger.settings");
 	if (settingsFile.is_open())
 	{
 		// Read each line and pass to settings module
    	while ( getline (settingsFile,line) )
    	{
      		Settings_readFromString(line.c_str());
    	}
    	settingsFile.close();

    	Settings_echoAllSet(localPrintFn);
  	}
    
    Settings_Init();

    std::cout << std::endl << "Reading using Settings_readGlobalFromFile by passing filename." << std::endl;

    LocalStorageInterface * s_storageIf = LocalStorage_GetLocalStorageInterface(LOCAL_STORAGE_TYPE(0));
    Settings_readGlobalFromFile(s_storageIf, "datalogger.settings");

    Settings_echoAllSet(localPrintFn);

    std::cout << std::endl << "Setting up a data manager based on global settings."  << std::endl;
    int32_t storageInterval = Settings_getInt(DATA_STORAGE_INTERVAL_SECS);
    int32_t averagingInterval = Settings_getInt(DATA_AVERAGING_INTERVAL_SECS);

    int32_t storageSize = (storageInterval / averagingInterval);
    if (storageInterval % averagingInterval != 0) { storageSize++; }

    int32_t averagerSize = averagingInterval * 10;

    std::cout << "For an application that:" << std::endl;
    std::cout << "- Takes 10 readings every second." << std::endl;
    std::cout << "- Averages those readings every " << averagingInterval << " seconds." << std::endl;
    std::cout << "- Stores those averages every " << storageInterval << " seconds." << std::endl;
    std::cout << "Each averager needs " << averagerSize << " elements." << std::endl;
    std::cout << "Each datafield needs to store " << storageSize << " averages." << std::endl;

    DataFieldManager manager = DataFieldManager(storageSize, averagerSize);

    std::cout << std::endl << "Reading channel setting using Settings_readChannelsFromFile." << std::endl;
    
    if (ERR_READER_NONE != Settings_readChannelsFromFile(&manager, s_storageIf, "datalogger.channels"))
    {
        std::cout << "Reading error: " << Settings_getLastReaderResultText() << std::endl;
        return 1;
    }

    uint32_t count = manager.count();
    std::cout << "A manager was created with " << count << " fields.";

    uint32_t i;

    for (i = 0; i < count; i++)
    {
        std::cout << "Field " << i << " type is " << manager.getField(i)->getTypeString() << ".";
    }
    return 0;
}
