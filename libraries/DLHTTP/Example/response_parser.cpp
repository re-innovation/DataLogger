/*
response_parser.cpp

A sample command-line application using the DLHTTP module

You can pipe the response of an HTTP request in to get the parsed output.
Build with:
g++ -I ../../DLUtility response_parser.cpp ../DLHTTP.Header.cpp ../DLHTTP.ResponseParser.cpp -o response_parser.exe
*/

#include <iostream>

#include "../../DLUtility/DLUtility.Strings.cpp"
#include "../DLHTTP.h"

std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}

int main(int argc, char * argv[])
{
    std::string response;
    
    if (argc == 1)
    {
        char response_buffer[1024];
        std::cin.getline(response_buffer, 1024);
        response = std::string(response_buffer);
    }
    else
    {
        response = std::string(argv[1]);
    }
    
    response = ReplaceAll(response, "\\r\\n", "\r\n");

    std::cout << "Parsing response:" <<std::endl;
    std::cout << response.c_str() << std::endl;
    
    ResponseParser respParser(response.c_str());
        
    std::cout << "Status: " << respParser.getStatus() << std::endl;
    std::cout << "HTTP Version: " << respParser.getVersion() << std::endl;
    
    std::cout << "Headers:" << std::endl;
    
    uint8_t i = 0;
    for (i = 0; i < respParser.headerCount(); i++)
    {
        std::cout << i <<  "- " << respParser.getHeaderByIndex(i)->getName() << ": " << respParser.getHeaderByIndex(i)->getValue() << std::endl;
    }
    
    return 0;
}