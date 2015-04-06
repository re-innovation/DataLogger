/*
 * linkitonegprs.cpp
 * 
 * Connect and communicate over GPRS
 *
 * Author: James Fowkes
 *
 * www.re-innovation.co.uk
 */

/*
 * Arduino/C++ Library Includes
 */

#include <Arduino.h>
#include <stdio.h>

/*
 * LinkIt One Includes
 */

#include <LSD.h>
#include <LStorage.h>

/*
 * DataLogger Includes
 */

#include "DLUtility.h"
#include "DLLocalStorage.h"
#include "DLLocalStorage.linkitonesd.h"

/*
 * Private Variables
 */

// Since the LinkIt ONE can only have one open file at a time, keep this as a file-local static member.
// The LinkItOne SD class then just acts as an interface to interact with this file object.
static LFile s_file;
static bool s_fileIsOpenForRead = false;
static bool s_fileIsOpenForWrite = false;

/*
 * Public Functions 
 */

LinkItOneSD::LinkItOneSD()
{
    LSD.begin(); // Start the LinkIt ONE SD Interface
    m_echo = false;
}

void LinkItOneSD::setEcho(bool set)
{
	m_echo = set;
}

bool LinkItOneSD::fileExists(char const * const filePath)
{
    return LSD.exists((char*)filePath);
}

bool LinkItOneSD::directoryExists(char const * const dirPath)
{
    return LSD.exists((char*)dirPath);
}

bool LinkItOneSD::mkDir(char const * const dirPath)
{
    return LSD.mkdir((char*)dirPath);
}

static char readOneByteFromFile(void)
{
	return s_file.available() ? s_file.read() : '\0';
}

FILE_HANDLE LinkItOneSD::openFile(char const * const filename, bool forWrite)
{
    s_file.close(); // Ensure previous file (if any) is closed
    s_file = LSD.open(filename, forWrite ? FILE_WRITE : FILE_READ);

    if (s_file)
    {
    	s_fileIsOpenForWrite = forWrite;
    	s_fileIsOpenForRead = !forWrite;
    }
    else
    {
    	s_fileIsOpenForWrite = false;
    	s_fileIsOpenForRead = false;
    }

    return 0; // The LinkIt ONE can only support one open file at a time, so no need to track file handles
}

void LinkItOneSD::write(FILE_HANDLE file, char const * const toWrite)
{
	(void)file; // The LinkIt ONE can only support one open file at a time, so discard handle
	bool fileAvailableForWrite = true;
	fileAvailableForWrite &= !s_file.isDirectory();
	fileAvailableForWrite &= s_fileIsOpenForWrite;

	if (fileAvailableForWrite)
	{
		s_file.print(toWrite);
		if (m_echo)
		{
			Serial.print(toWrite);
		}
	}
	else
	{
		Serial.println("File not open for write!");
	}
}

uint32_t LinkItOneSD::readBytes(FILE_HANDLE file, char * buffer, uint32_t n)
{
	(void)file; // The LinkIt ONE can only support one open file at a time, so discard handle
	bool fileAvailableForRead = true;
	fileAvailableForRead &= !s_file.isDirectory();
	fileAvailableForRead &= s_fileIsOpenForRead;

	if (fileAvailableForRead && buffer)
	{
		return s_file.read(buffer, n);
	}
}

uint32_t LinkItOneSD::readLine(FILE_HANDLE file, char * buffer, uint32_t n, bool stripCRLF)
{
	(void)file; // The LinkIt ONE can only support one open file at a time, so discard handle
	bool fileAvailableForRead = true;
	fileAvailableForRead &= !s_file.isDirectory();
	fileAvailableForRead &= s_fileIsOpenForRead;

	uint32_t readCount = 0;
	bool eol = false;

	if (fileAvailableForRead && buffer)
	{
		readLineWithReadFunction(readOneByteFromFile, buffer, n, stripCRLF);
	}

	return readCount;
}

bool LinkItOneSD::endOfFile(FILE_HANDLE file)
{
	(void)file; // The LinkIt ONE can only support one open file at a time, so discard handle
	return !s_file.available();	
}

void LinkItOneSD::closeFile(FILE_HANDLE file)
{
    (void)file; // The LinkIt ONE can only support one open file at a time, so discard handle
    s_file.close();
    s_fileIsOpenForWrite = false;
    s_fileIsOpenForRead = false;
}
