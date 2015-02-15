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
}

bool LinkItOneSD::FileExists(char * filePath)
{
    return LSD.exists(filePath);
}

bool LinkItOneSD::DirectoryExists(char * dirPath)
{
    return LSD.exists(dirPath);
}

bool LinkItOneSD::MkDir(char * dirPath)
{
    return LSD.mkdir(dirPath);
}

char ReadOneByteFromFile(void)
{
	return s_file.available() ? s_file.read() : '\0';
}

FILE_HANDLE LinkItOneSD::OpenFile(char * filename, bool forWrite)
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

void LinkItOneSD::Write(FILE_HANDLE file, char * toWrite)
{
	(void)file; // The LinkIt ONE can only support one open file at a time, so discard handle
	bool fileAvailableForWrite = true;
	fileAvailableForWrite &= !s_file.isDirectory();
	fileAvailableForWrite &= s_fileIsOpenForWrite;

	if (fileAvailableForWrite)
	{
		s_file.print(toWrite);
	}
}

uint32_t LinkItOneSD::ReadBytes(FILE_HANDLE file, char * buffer, uint32_t n)
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

uint32_t LinkItOneSD::ReadLine(FILE_HANDLE file, char * buffer, uint32_t n)
{
	(void)file; // The LinkIt ONE can only support one open file at a time, so discard handle
	bool fileAvailableForRead = true;
	fileAvailableForRead &= !s_file.isDirectory();
	fileAvailableForRead &= s_fileIsOpenForRead;

	uint32_t readCount = 0;
	bool eol = false;

	if (fileAvailableForRead && buffer)
	{
		ReadLineWithReadFunction(ReadOneByteFromFile, buffer, n);
	}

	return readCount;
}

bool LinkItOneSD::EndOfFile(FILE_HANDLE file)
{
	(void)file; // The LinkIt ONE can only support one open file at a time, so discard handle
	return !s_file.available();	
}

void LinkItOneSD::CloseFile(FILE_HANDLE file)
{
    (void)file; // The LinkIt ONE can only support one open file at a time, so discard handle
    s_file.close();
    s_fileIsOpenForWrite = false;
    s_fileIsOpenForRead = false;
}
