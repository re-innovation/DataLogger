/*
DLService.thingspeak.example.cpp

A sample command-line application using the DLHTTP module

You can pipe the output of this example to nc to get responses from URLs

Build and run by just typing "make" at command line

The program will create a POST request and echo it to stdout.
*/

#include <iostream>

#include "DLSettings.h"
#include "DLUtility.Strings.h"
#include "DLService.h"


char csvData[] = 
"2015-02-13 07:12:22 +0000,1,43.478,51.752,4.90,5.23,9.23,2.84\r\n\
2015-02-13 07:12:52 +0000,2,49.321,54.782,9.63,5.01,7.30,8.63\r\n\
2015-02-13 07:13:22 +0000,3,51.023,42.647,7.57,6.89,8.24,0.52\r\n\
2015-02-13 07:13:52 +0000,4,54.194,59.884,7.68,9.67,5.35,6.02\r\n\
";

int main(int argc, char * argv[])
{
    (void)argc;
    (void)argv;

    char request_buffer[2048];
    Settings_setString(THINGSPEAK_URL, "agile-headland-8076.herokuapp.com");
    Settings_setString(THINGSPEAK_API_KEY, "IZ2O45C3BM257VCH");
    
    ServiceInterface * thingspeak = Service_GetService(SERVICE_THINGSPEAK);

    thingspeak->createBulkUploadCall(request_buffer, 1024, csvData, "example.csv");
    std::cout << request_buffer;
    
    return 0;
}
