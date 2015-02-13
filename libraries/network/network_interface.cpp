/*
 * network_interface.cpp
 * 
 * Connect and communicate with network resources
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

#include "settings.h"

/*
 * Public Functions 
 */

NetworkInterface * CreateInterface(NETWORK_INTERFACE interface)
{
    NetworkInterface * pInterface = NULL;
    switch(interface)
    {
    case NETWORK_INTERFACE_LINKITONE_WIFI:
        pInterface = new LinkItOneWifi();
    case NETWORK_INTERFACE_LINKITONE_GPRS
        char * pAPN = GetSetting(GRPS_APN);
        char * pUser = GetSetting(GRPS_USERNAME);
        char * pPwd = GetSetting(GRPS_PASSWORD);
        pInterface = new LinkItOneGPRS(pAPN, pUser, pPwd);
    }
    return pInterface;
}
