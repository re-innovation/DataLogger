/*
 * DLService.Thingspeak.cpp
 * 
 * Tests the Thingspeak request generation functionality
 *
 * Author: James Fowkes
 *
 * www.re-innovation.co.uk
 */

/*
 * C++ Library Includes
 */

#include <string.h>
#include <stdint.h>
#include <iostream>
#include <string>
#include <vector>

/*
 * Local Application Includes
 */

#include "DLSettings.h"
#include "DLUtility.h"
#include "DLService.h"
#include "DLService.Thingspeak.h"

/*
 * Unity Test Framework
 */

#include "unity.h"

char expectedResponseLines[20][89] = { 
	"POST /update_csv HTTP/1.1\r\n",
	"Host: api.thingspeak.com\r\n",
	"Content-Type: multipart/form-data; boundary=----------------------9f1bb96494379c3e\r\n",
	"Content-Length: 629\r\n",
	"\r\n",
	"------------------------9f1bb96494379c3e\r\n",
	"Content-Disposition: form-data; name=\"api_key\"\r\n",
	"\r\n",
	"IZ2O45C3BM257VCH\r\n",
	"------------------------9f1bb96494379c3e\r\n",
	"Content-Disposition: form-data; name=\"upload[csv]\"; filename=\"example.csv\"\r\n",
	"Content-Type: application/octet-stream\r\n",
	"\r\n",
	"created_at,entry_id,field1,field2,field3,field4,field5,field6\r\n",
	"2015-02-13 07:12:22 +0000,1,43.478,51.752,4.90,5.23,9.23,2.84\r\n",
	"2015-02-13 07:12:52 +0000,2,49.321,54.782,9.63,5.01,7.30,8.63\r\n",
	"2015-02-13 07:13:22 +0000,3,51.023,42.647,7.57,6.89,8.24,0.52\r\n",
	"2015-02-13 07:13:52 +0000,4,54.194,59.884,7.68,9.67,5.35,6.02\r\n",
	"\r\n",
	"------------------------9f1bb96494379c3e--\r\n",
};

char csvData[] = 
"2015-02-13 07:12:22 +0000,1,43.478,51.752,4.90,5.23,9.23,2.84\r\n\
2015-02-13 07:12:52 +0000,2,49.321,54.782,9.63,5.01,7.30,8.63\r\n\
2015-02-13 07:13:22 +0000,3,51.023,42.647,7.57,6.89,8.24,0.52\r\n\
2015-02-13 07:13:52 +0000,4,54.194,59.884,7.68,9.67,5.35,6.02\r\n\
";

char requestBuffer[1024];
std::vector<std::string> requestStrings;

uint8_t s_testLine;

void createRequest()
{
    Settings_setString(THINGSPEAK_URL, "api.thingspeak.com");
    Settings_setString(THINGSPEAK_API_KEY, "IZ2O45C3BM257VCH");
    
    ServiceInterface * thingspeak = Service_GetService(SERVICE_THINGSPEAK);

    thingspeak->createBulkUploadCall(requestBuffer, 1024, csvData, "example.csv");

    // Go through request buffer and split into strings    
	size_t pos = 0;
	std::string token;
	std::string delimiter = "\r\n";
	std::string s = std::string(requestBuffer);

	requestStrings.clear();
	
	while ((pos = s.find(delimiter)) != std::string::npos)
	{
	    token = s.substr(0, pos) + delimiter;
	 	requestStrings.push_back(token);
    	s.erase(0, pos + delimiter.length());
    }
}   

void test_BulkUploadRequestHasCorrectNumberOfLines(void)
{
    TEST_ASSERT_EQUAL(20, requestStrings.size());
}

void test_BulkUploadRequestLineIsCorrect()
{
	char msg[32];
	sprintf(msg, "Request line %d", s_testLine+1);
	TEST_ASSERT_EQUAL_STRING_MESSAGE(expectedResponseLines[s_testLine], requestStrings[s_testLine].c_str(), msg);
}

void test_TimestampsAreCreatedSuccessfully(void)
{
	TM testTime;

	testTime.tm_year = 115; // 2015 as years since 1900
	testTime.tm_mon = 4;
	testTime.tm_mday = 3;
	testTime.tm_hour = 13;
	testTime.tm_min = 9;
	testTime.tm_sec = 34;

	char buffer[100];

	Thingspeak::writeTimestampToBuffer(&testTime, buffer);

	TEST_ASSERT_EQUAL_STRING("2015-04-03 13:09:34 +0000", buffer);
}

int main(void)
{
    UnityBegin("DLService.Thingspeak.cpp");

	createRequest();

    RUN_TEST(test_BulkUploadRequestHasCorrectNumberOfLines);
    
 	for (s_testLine = 0; s_testLine < requestStrings.size(); ++s_testLine)
 	{

 		RUN_TEST(test_BulkUploadRequestLineIsCorrect);
 	}

 	RUN_TEST(test_TimestampsAreCreatedSuccessfully);
    return 0;
}
