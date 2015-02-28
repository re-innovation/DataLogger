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
 * DLHTTP.ResponseParser.cpp
 *
 * This class handles creation parsing of HTTP responses.
 * It is a heavily stripped-down and altered version of HappyHTTP by Ben Campbell (see above).
 */

/*
 * C++ Library Includes
 */

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#endif
/*
 * Local Application Includes
 */

#include "DLUtility.Strings.h"
#include "DLHTTP.h"

//---------------------------------------------------------------------
//
// ResponseParser
//
//---------------------------------------------------------------------

ResponseParser::ResponseParser(const char * response) : m_headerCount(0), m_lineAccumulator(m_headerBuffer, MAX_HTTP_HEADER_TOTAL_LENGTH)
{
    m_State = STATUSLINE;
    m_version = 0;
    m_status = 0;
    m_BytesRead = 0;
    m_Length = -1;
    m_headerCount = 0;
    m_lineAccumulator.attach(&m_headerBuffer[0], MAX_HTTP_HEADER_TOTAL_LENGTH);

    uint16_t count = strlen(response);

	while( count > 0 && m_State != COMPLETE )
	{
		if( m_State == STATUSLINE ||
			m_State == HEADERS
			)
		{
			// we want to accumulate a line
			while( count > 0 )
			{
				char c = (char)*response++;
				--count;
				if( c == '\n' )
				{
					// now got a whole line!
					switch( m_State )
					{
						case STATUSLINE:
							processStatusLine( m_lineAccumulator.c_str() );
							break;
						case HEADERS:
							processHeaderLine( m_lineAccumulator.c_str() );
							break;
						default:
							break;
					}
					m_lineAccumulator.reset();
					break;		// break out of line accumulation!
				}
				else
				{
					if( c != '\r' )		// just ignore CR
						m_lineAccumulator.writeChar(c);
				}
			}
		}
		else if( m_State == BODY )
		{
			int bytesused = 0;
            bytesused = processBody( response, count );
			response += bytesused;
			count -= bytesused;
		}
	}    
}

/*
 * findHeaderInList
 *
 * Searches m_headers list until header with this name is found or the list ends.
 * Returns pointer to found header or NULL if not found.
 */

Header * ResponseParser::findHeaderInList(char * name)
{

	if (!name) { return NULL; }
    
    toLowerStr(name);

    uint8_t i;
    Header * found = NULL;
    for (i = 0; i < m_headerCount; i++)
    {
        if (m_headers[i].matchName(name))
        {
            found = &m_headers[i];
            break;
        }
    }
    
    return found;
}

const char* ResponseParser::getHeaderValue(char  * name )
{
    if (!name) { return NULL; }
    
    toLowerStr(name);
    
    Header * header = findHeaderInList(name);
    
	return header ? header->getValue() : NULL;
}

// handle some body data
// returns number of bytes used.
int ResponseParser::processBody( const char * data, int count )
{
    (void)data;
	int n = count;
	if( m_Length != -1 )
	{
		// we know how many bytes to expect
		int remaining = m_Length - m_BytesRead;
		if( n > remaining )
			n = remaining;
	}

	m_BytesRead += n;

	// Finish if we know we're done
	if( m_Length != -1 && m_BytesRead == m_Length )
	{
		finish();
	}

	return n;
}


void ResponseParser::finish()
{
	m_State = COMPLETE;
}


void ResponseParser::processStatusLine( const char * line )
{
	// skip any leading space
    line = skipSpaces(line);
    
    // Get version (ASSUMES that string is "HTTP/x.x")
    if (strncmp("HTTP/1.", line, 7) == 0)
    {
        m_version = line[7] == '0' ? 10 : 11;
    }
    else
    {
        m_version = 0;
    }
    line += 8;
    line = skipSpaces(line);
    
    char status[4];
    
    // ASSUMES status code is exactly three digits long!
    status[0] = *line++;
    status[1] = *line++;
    status[2] = *line++;
    status[3] = '\0';
    m_status = atoi( status );
    
    line = skipSpaces(line);
    
    // rest of line is reason
    uint8_t i = 0;
    while( *line != '\r' )
    {
        m_reason[i++] = *line++;
    }

    /*
	printf( "version: '%d'\n", m_version );
	printf( "status: '%d'\n", m_status );
	printf( "reason: '%s'\n", m_reason );
	*/

	// OK, now we expect headers!
	m_State = HEADERS;
	clearHeaderAccumulator();
}

void ResponseParser::processHeaderLine( const char * line )
{
	if( strlen(line) == 0 )
	{

		storeHeader();	// end of headers
		beginBody();	// start on body now!
        
        return;
	}

	if( !isspace(*line) )
	{
		// begin a new header
		storeHeader();
		m_lineAccumulator.writeString(line);
	}
	else
	{
		// This is a continuation line - just add it to previous data
		++line;
		
        line = skipSpaces(line);
        
        m_lineAccumulator.writeChar(' ');
		m_lineAccumulator.writeString(line);
	}
}

// process accumulated header data
void ResponseParser::storeHeader()
{
	if( strlen(m_lineAccumulator.c_str()) == 0) { return; }

	m_headers[m_headerCount++].setFromLine(m_lineAccumulator.c_str());
    
//	printf("header: ['%s': '%s']\n", header.c_str(), value.c_str() );	

	clearHeaderAccumulator();
}

void ResponseParser::clearHeaderAccumulator(void)
{
    m_lineAccumulator.reset();
}

// OK, we've now got all the headers read in, so we're ready to start
// on the body. But we need to see what info we can glean from the headers
// first...
void ResponseParser::beginBody()
{

	m_Length = -1;	// unknown
    
	// length supplied?
	const char* contentlen = getHeaderValue( (char*)"content-length" );
	if( contentlen )
	{
		m_Length = atoi( contentlen );
	}

	// check for various cases where we expect zero-length body
	if( m_status == NO_CONTENT ||
		m_status == NOT_MODIFIED ||
		( m_status >= 100 && m_status < 200 ))		// 1xx codes have no body
	{
		m_Length = 0;
	}

/*
	printf("---------BeginBody()--------\n");
	printf("Length: %d\n", m_Length );
	printf("----------------------------\n");
*/
	// now start reading body data!
	m_State = BODY;
}

int ResponseParser::getVersion() const
{
	// only valid once we've got the statusline
	return m_version;
}

int ResponseParser::getStatus() const
{
	// only valid once we've got the statusline
	return m_status;
}

const char* ResponseParser::getReason() const
{
	// only valid once we've got the statusline
	return m_reason;
}

int ResponseParser::headerCount() const
{
	return m_headerCount;
}