/*
request_builder.cpp

A sample command-line application using the DLHTTP module

You can pipe the output of this example to cURL to get responses from URLs

Build with:
g++ -I ../../DLUtility request_builder.cpp ../DLHTTP.Header.cpp ../DLHTTP.RequestBuilder.cpp -o request_builder.exe

*/

#include <iostream>

#include "../../DLUtility/DLUtility.Strings.cpp"
#include "../DLHTTP.h"

int main(int argc, char * argv[])
{
    char request_buffer[2048];
    char * name;
    char * value;
    
    RequestBuilder builder;
    
    char * url = argv[1];
    char * method = argv[2];
    char * header_list = argv[3];
    
    char * body = NULL;
    if (argc == 5) { body = argv[4]; }
    
    if (argc < 4)  { return 0; }
    
    builder.setMethodAndURL(method, url);
   
    name = strtok(header_list, ",");
    value = strtok(NULL, ",");
    
    while ((name != NULL) && (value != NULL))
    {
        builder.putHeader(name, value);
        name = strtok(NULL, ",");
        value = strtok(NULL, ",");
    }
    
    if (body)
    {
        builder.putBody(body);
    }
    
    builder.writeToBuffer(request_buffer, 2048, true);
    
    std::cout << request_buffer << std::endl;
    
    return 0;
}
