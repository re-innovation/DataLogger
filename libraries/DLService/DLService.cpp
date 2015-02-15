/*
 * service_interface.cpp
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
            char * key = Settings_GetString(THINGSPEAK_API_KEY);
            return new Thingspeak(key);
    }
 }