/*
 * DLService.cpp
 * 
 * Pushes/pulls data from internet bases services
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
#include <stdlib.h>
#endif

/*
 * Local Application Includes
 */

#include "DLUtility.h"
#include "DLSettings.h"
#include "DLService.h"
#include "DLService.thingspeak.h"

/*
 * Private Variables
 */

/*
 * Private Functions 
 */
 
/*
 * Public Functions 
 */

 ServiceInterface * Service_GetService(SERVICE service)
 {
    switch (service)
    {
        case SERVICE_THINGSPEAK:
        	char * url = Settings_getString(THINGSPEAK_URL);
            char * key = Settings_getString(THINGSPEAK_API_KEY);
            return new Thingspeak(url, key);
    }

    // If here, no service found
    return NULL;
 }