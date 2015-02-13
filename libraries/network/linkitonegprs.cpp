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

/*
 * LinkIt One Includes
 */

#include <LGPRS.h>
#include <LGPRSClient.h>

/*
 * Private Defines and Typedefs
 */

#define HTTP_PORT 80

/*
 * Private Variables
 */

/*
 * Public Functions 
 */

LinkItOneGPRS::LinkItOneGPRS(char * apn, char * username, char * password)
{
    m_pAPN = apn;
    m_pUser = username;
    m_pPwd = password;
    m_client = new LGPRSClient(); 
}

LinkItOneGPRS::~LinkItOneGPRS() {}

bool LinkItOneGPRS::tryConnection(uint8_t timeoutSeconds)
{
    unsigned long timeout = millis() + (timeoutSeconds*1000);
    
    bool success = false;
    while (!success && (millis() < timeout))
    {
        success = LGPRS.attachGPRS(m_pAPN, m_pUser, m_pPwd);
    }
    return success;
}

bool LinkItOneGPRS::HTTPGet(char * url, char * request, bool useHTTPS)
{
    bool success = m_client.connect(url, HTTP_PORT);
    if (success)
    {
        client.print("GET ");
        client.print(request);
        client.println(" HTTP/1.1");
        client.print("Host: ");
        client.println(url);
        client.println("Connection: close");
        client.println();
    }
    return success;
}
