/*
 * DLLocalStorage.cpp
 * 
 * Provides interface to local storage devices (SD, USB, etc.)
 *
 * Author: James Fowkes
 *
 * www.re-innovation.co.uk
 */

/*
 * Arduino/C++ Library Includes
 */

#ifdef ARDUINO
#include <Arduino.h>
#else 
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#endif

/*
 * Local Application Includes
 */

#include "DLLocalStorage.h"
#include "DLLocalStorage.linkitonesd.h"
/*
 * Public Functions 
 */

LocalStorageInterface * LocalStorage_GetLocalStorageInterface(LOCAL_STORAGE_TYPE storage_type)
{
	#ifndef TEST
	switch (storage_type)
	{
	case LINKITONE_SD_CARD:
		return new LinkItOneSD();
	default:
		return NULL;
	}
	#else
	(void)storage_type;
	return NULL;
	#endif
}
