/*
DLSettings.Reader.example.cpp

A sample command-line application showing usage of the settings file reader

Reads from "datalogger.settings" and echoes those settings back to the command line

*/

#include <iostream>
#include <fstream>
#include <string>

#include "DLSettings.h"
#include "DLSettings.Reader.h"

int main(int argc, char * argv[])
{
    (void)argc;
    (void)argv;

    std::string line;
  	std::ifstream settingsFile("datalogger.settings");
 	if (settingsFile.is_open())
 	{
 		// Read each line and pass to settings module
    	while ( getline (settingsFile,line) )
    	{
      		Settings_ReadFromString(line.c_str());
    	}
    	settingsFile.close();

    	// Echo out integer settings
    	int i;
    	for (i = 0; i < INT_SETTINGS_COUNT; i++)
    	{
    		std::cout << Settings_getIntName((INTSETTING)i) << ": " << Settings_getInt((INTSETTING)i) << std::endl;
    	}

    	// Echo out string settings
    	for (i = 0; i < STRING_SETTINGS_COUNT; i++)
    	{
    		std::cout << Settings_getStringName((STRINGSETTING)i) << ": " << Settings_getString((STRINGSETTING)i) << std::endl;
    	}
  	}
    
    return 0;
}
