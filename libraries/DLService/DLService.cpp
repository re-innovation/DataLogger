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

#include <Arduino.h>

/*
 * Local Application Includes
 */

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
 }