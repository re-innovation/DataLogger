/*
request_builder.cpp

A sample command-line application using the DLHTTP module

You can pipe the output of this example to nc to get responses from URLs

Build with:
g++ -I ../../DLUtility request_builder.cpp ../DLHTTP.Header.cpp ../DLHTTP.RequestBuilder.cpp -o request_builder.exe

For GET requests:
request_builder.exe api.thingspeak.com GET "api_key,WHATEVERAPIKEY,field1,10.00,field2,12.00"

For POST requests:
request_builder.exe api.thingspeak.com POST "Host,api.thingspeak.com,Connection,close" "content"
*/

#include <iostream>
#include <string.h>

#include "../../DLUtility/DLUtility.Strings.h"
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

    char * getURL = new char[strlen(url)];
    
    char * body = NULL;
    if (argc == 5) { body = argv[4]; }
    
    if (argc < 4)  { return 0; }
    
   
    if (strncmp(method, "GET", 3) == 0)
    {
        // Add URL to headers as host for get, and a connection close
        
        char * slash = strchr(url, '/');
        if (slash)
        {
            builder.setMethodAndURL(method, slash);
            strncpy_safe(getURL, url, slash - url);
            builder.putHeader("Host", getURL);
        }
        else
        {
            builder.setMethodAndURL(method, url);
            builder.putHeader("Host", url);
        }
        builder.putHeader("Connection", "keep-alive");
    }
    else
    {
        builder.setMethodAndURL(method, url);
    }
    
    name = strtok(header_list, ",");
    value = strtok(NULL, ",");
    
    while ((name != NULL) && (value != NULL))
    {
        if (strncmp(method, "GET", 3) == 0)
        {   
            builder.setURLParam(name, value);
        }
        else
        {
            builder.putHeader(name, value);
        }
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
