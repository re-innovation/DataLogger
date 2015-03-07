/*
 * HappyHTTP - a simple HTTP library
 * Version 0.1
 * 
 * Copyright (c) 2006 Ben Campbell
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software in a
 * product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not
 * be misrepresented as being the original software.
 * 
 * 3. This notice may not be removed or altered from any source distribution.
 *
 */

/* 
 * DLHTTP.h
 *
 * This module handles creation of HTTP requests and parsing of responses.
 * It is a heavily stripped-down and altered verison of HappyHTTP by Ben Campbell (see above).
 * Principal changes made:
 *  - Removed HTTP connection functionality, since this will be dealt with by other libraries
 *  - Removed all C++ STL functionality
 *  - Removed HTTP chunked data functionality
 *  - Removed stubbed HTTP trailers functionality
 *  - Lots of variable length things are now fixed length, e.g. the number of headers is fixed at MAX_HTTP_HEADERS.
        This is because this library is intended for use in embedded applications where the requests/responses
        are known, small and fixed.
 */

#ifndef _DL_HTTP_H_
#define _DL_HTTP_H_

/*
 * Defines and typedefs
 */

#define MAX_HTTP_HEADERS                (10) // Number of headers that can be sent/recvd in a single request
#define MAX_HTTP_URL_PARAMS             (10) // Number of URL parameters that can be sent in a single request

#define MAX_HOST_LENGTH                 (30) // Maximum length of host URL

#define MAX_HTTP_HEADER_NAME_LENGTH     (30) // Maximum length of single header name
#define MAX_HTTP_HEADER_VALUE_LENGTH    (80) // Maximum length of single header value
#define MAX_HTTP_RESPONSE_REASON_LENGTH (40) // Maximum length of HTTP response reason string

// Maximum header length: assume a maximum of 10 chars extra on top of name and value
#define MAX_HTTP_HEADER_TOTAL_LENGTH (MAX_HTTP_HEADER_NAME_LENGTH + MAX_HTTP_HEADER_VALUE_LENGTH + 10)

// HTTP status codes
enum {
    // 1xx informational
    CONTINUE = 100,
    SWITCHING_PROTOCOLS = 101,
    PROCESSING = 102,

    // 2xx successful
    OK = 200,
    CREATED = 201,
    ACCEPTED = 202,
    NON_AUTHORITATIVE_INFORMATION = 203,
    NO_CONTENT = 204,
    RESET_CONTENT = 205,
    PARTIAL_CONTENT = 206,
    MULTI_STATUS = 207,
    IM_USED = 226,

    // 3xx redirection
    MULTIPLE_CHOICES = 300,
    MOVED_PERMANENTLY = 301,
    FOUND = 302,
    SEE_OTHER = 303,
    NOT_MODIFIED = 304,
    USE_PROXY = 305,
    TEMPORARY_REDIRECT = 307,
    
    // 4xx client error
    BAD_REQUEST = 400,
    UNAUTHORIZED = 401,
    PAYMENT_REQUIRED = 402,
    FORBIDDEN = 403,
    NOT_FOUND = 404,
    METHOD_NOT_ALLOWED = 405,
    NOT_ACCEPTABLE = 406,
    PROXY_AUTHENTICATION_REQUIRED = 407,
    REQUEST_TIMEOUT = 408,
    CONFLICT = 409,
    GONE = 410,
    LENGTH_REQUIRED = 411,
    PRECONDITION_FAILED = 412,
    REQUEST_ENTITY_TOO_LARGE = 413,
    REQUEST_URI_TOO_LONG = 414,
    UNSUPPORTED_MEDIA_TYPE = 415,
    REQUESTED_RANGE_NOT_SATISFIABLE = 416,
    EXPECTATION_FAILED = 417,
    UNPROCESSABLE_ENTITY = 422,
    LOCKED = 423,
    FAILED_DEPENDENCY = 424,
    UPGRADE_REQUIRED = 426,

    // 5xx server error
    INTERNAL_SERVER_ERROR = 500,
    NOT_IMPLEMENTED = 501,
    BAD_GATEWAY = 502,
    SERVICE_UNAVAILABLE = 503,
    GATEWAY_TIMEOUT = 504,
    HTTP_VERSION_NOT_SUPPORTED = 505,
    INSUFFICIENT_STORAGE = 507,
    NOT_EXTENDED = 510,
};

//-------------------------------------------------
// URLParam
//
// Simple struct to hold two pointers for name 
// and value of a URL parameter
// ------------------------------------------------
struct urlparam
{
    const char * name;
    const char * value;
};
typedef struct urlparam URLParam;

//-------------------------------------------------
// Header
//
// Handles building of HTTP headers.
// ------------------------------------------------

class Header
{
    public:
        Header();
        ~Header();
    
        void setFromLine(const char * lineToParse);
        void setName(const char * name);
        void setValue(const char * value);
        char * getValue(void);
        char * getName(void);
        
        bool matchName(const char * name);
    
    private:
        char m_name[MAX_HTTP_HEADER_NAME_LENGTH];
        char m_value[MAX_HTTP_HEADER_VALUE_LENGTH];
};

//-------------------------------------------------
// ResponseParser
//
// Handles parsing of response data.
// ------------------------------------------------

class ResponseParser
{
    public:

        ResponseParser(const char * response);
        
        // retrieve a header (returns 0 if not present)
        const char* getHeaderValue(char* name );
        
        Header * getHeaderByIndex(uint8_t index);
        Header * findHeaderInList(char * name);

        int getStatus() const;          // get the HTTP status code
        int getVersion() const;         // get the HTTP version code
        const char* getReason() const;  // get the HTTP response reason string
        int headerCount() const;

    private:
        enum {
            STATUSLINE,		// start here. status line is first line of response.
            HEADERS,		// reading in header lines
            BODY,			// waiting for some body data
            COMPLETE,		// response is complete!
        } m_State;

        char * m_method;		// req method: "GET", "POST" etc...

        // status line
        int	m_version;			                        // 10: HTTP/1.0    11: HTTP/1.x (where x>=1)
        int m_status;			                        // Status-Code
        char m_reason[MAX_HTTP_RESPONSE_REASON_LENGTH]; // Reason-Phrase

        
        int		m_BytesRead;		// body bytes read so far
        int		m_Length;			// -1 if unknown

        // header/value pairs
        Header m_headers[MAX_HTTP_HEADERS];

        // Accumulation buffer for headers.
        uint8_t m_headerCount;
        char m_headerBuffer[MAX_HTTP_HEADER_TOTAL_LENGTH+1];
        
        FixedLengthAccumulator m_lineAccumulator; // line accumulation for states that want it

        void storeHeader();
        void processStatusLine( const char * line );
        void processHeaderLine( const char * line );

        int processBody( const char* data, int count );
        
        void beginBody();
        void finish();
        
        void clearHeaderAccumulator(void);
};

class RequestBuilder
{
    public:
        RequestBuilder();
        ~RequestBuilder();
        
        void setMethodAndURL(const char* method, const char* url);
        void setURLParam( const char* name, const char* value );

        void putHeader(const char* name, const char* value);
        void putBody(const char * body);
        
        void writeToBuffer(char * buf, uint16_t maxLength, bool addContentLengthHeader = false);
        
        void reset(void);
        
    private:
        // HTTP header name/value pairs
        Header m_headers[MAX_HTTP_HEADERS];
        uint8_t m_headerCount;

        // URL param name/value pairs
        URLParam m_params[MAX_HTTP_URL_PARAMS];
        uint8_t m_paramCount; 

        const char * m_method;
        const char * m_url;
        const char * m_body;
        
        FixedLengthAccumulator accumulator;

};

#endif // _DL_HTTP_H_
