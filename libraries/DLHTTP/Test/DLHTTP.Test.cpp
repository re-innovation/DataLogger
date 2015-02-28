/*
 * DLHTTP.Test.cpp
 * 
 * Tests the HTTP Request and Response class
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

/*
 * Local Application Includes
 */

#include "DLUtility.Strings.h"
#include "../DLHTTP.h"

/*
 * Unity Test Framework
 */

#include "unity.h"

char requestBuffer[512];
RequestBuilder builder(requestBuffer, 512);
        
void test_header_isSuccessfulyParsed(void)
{
    Header header;
    
    header.setFromLine("Content-Length: 48");
    
    TEST_ASSERT_TRUE(header.matchName("content-length"));
    TEST_ASSERT_EQUAL_STRING("content-length", header.getName());
    TEST_ASSERT_EQUAL_STRING("48", header.getValue());
}

void test_requestbuilder_BuildsWithMethodAndURLOnly(void)
{
    builder.setMethodAndURL("GET", "www.example.com");   
    TEST_ASSERT_EQUAL_STRING("GET www.example.com HTTP/1.1\r\n", requestBuffer);
}

void test_requestbuilder_BuildsWithHeaders(void)
{
    builder.putHeaders("Content-Length", "48");
    builder.putHeaders("Some-Other-Header", "Some-Other-Value");
    TEST_ASSERT_EQUAL_STRING(
        "GET www.example.com HTTP/1.1\r\n"
        "Content-Length: 48\r\n"
        "Some-Other-Header: Some-Other-Value\r\n", requestBuffer);
}

void test_requestbuilder_BuildsWithBodyContent(void)
{
    builder.putBody("This is some data in the body.");
    TEST_ASSERT_EQUAL_STRING(
        "GET www.example.com HTTP/1.1\r\n"
        "Content-Length: 48\r\n"
        "Some-Other-Header: Some-Other-Value\r\n"
        "\r\n"
        "This is some data in the body.\r\n", requestBuffer);
}

void test_responseparser_ReadsHTTPStatusLine(void)
{
    char statusLine[] = "HTTP/1.0 200 OK\r\n";

    ResponseParser response(statusLine);

    TEST_ASSERT_EQUAL(10, response.getVersion() );
    TEST_ASSERT_EQUAL(200, response.getStatus() );
    TEST_ASSERT_EQUAL_STRING("OK", response.getReason() );
}

int main(void)
{
    UnityBegin("DLHTTP.cpp");

    RUN_TEST(test_header_isSuccessfulyParsed);

    RUN_TEST(test_requestbuilder_BuildsWithMethodAndURLOnly);
    RUN_TEST(test_requestbuilder_BuildsWithHeaders);
    RUN_TEST(test_requestbuilder_BuildsWithBodyContent);

    RUN_TEST(test_responseparser_ReadsHTTPStatusLine);
    
    return 0;
}
