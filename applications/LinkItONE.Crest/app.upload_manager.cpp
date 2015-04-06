/*
 * upload_manager.cpp
 *
 * Upload manager for Crest PV application
 *
 * Basic summary:
 *
 * - Stores last uploaded data point and file so uploads can be tracked
 */

/*
 * Arduino/Standard Library Includes
 */

#include <Arduino.h>

/*
 * Datalogger Library Includes
 */

#include "DLUtility.h"

/*
 * Private variables
 */

static char s_lastFilename[100];
static uint32_t s_lastLine = 0;

void UploadMgr_logLastUpload(char * filename, uint32_t index)
{
	if (filename)
	{
		strncpy_safe(s_lastFilename, filename, 100);
		s_lastLine = index;
	}
}

void UploadMgr_getFilename(char * buffer, uint8_t maxLength)
{
	if (buffer) { strncpy_safe(buffer, s_lastFilename, maxLength); }
}

uint32_t UploadMgr_getLastLine(void)
{
	return s_lastLine;
}
