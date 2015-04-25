/*
DLSettings.Reader.example.cpp

A sample command-line application showing usage of the settings file reader

Reads from "datalogger.settings" and echoes those settings back to the command line

*/

#include <iostream>
#include <fstream>
#include <string>

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

    std::cout << "Reading using Settings_readFromString" << std::endl;
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

    std::cout << std::endl << "Reading using Settings_readFromFile" << std::endl;

    LocalStorageInterface * s_storageIf = LocalStorage_GetLocalStorageInterface(LOCAL_STORAGE_TYPE(0));
    Settings_readFromFile(s_storageIf, "datalogger.settings");

    Settings_echoAllSet(localPrintFn);

    return 0;
}
