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

#include "settings/settings.h"
#include "service/service_interface.h"
#include "service/thingspeak.h"

/*
 * Private Variables
 */

/*
 * Private Functions 
 */
 
/*
 * Public Functions 
 */

 ServiceInterface * GetService(SERVICE service)
 {
    switch (service)
    {
        case SERVICE_THINGSPEAK:
            char * key = GetString(THINGSPEAK_API_KEY);
            return ThingSpeak(key);
    }
 }