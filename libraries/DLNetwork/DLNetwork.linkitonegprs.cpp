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

#include "DLNetwork.h"
#include "DLNetwork.linkitone.h"

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
    m_connected = false;
    m_client = new LGPRSClient(); 
}

LinkItOneGPRS::~LinkItOneGPRS() {}

bool LinkItOneGPRS::tryConnection(uint8_t timeoutSeconds)
{
    unsigned long timeout = millis() + (timeoutSeconds*1000);
    
    m_connected = false;
    while (!m_connected && (millis() < timeout))
    {
        m_connected = LGPRS.attachGPRS(m_pAPN, m_pUser, m_pPwd);
    }
    return m_connected;
}

bool LinkItOneGPRS::HTTPGet(char const * const url, char * request, char * response, bool useHTTPS)
{
    bool success = m_connected;

    if (success)
    {
        success &= m_client->connect(url, HTTP_PORT);
        if (success)
        {
            m_client->print("GET ");
            m_client->print(request);
            m_client->println(" HTTP/1.1");
            m_client->print("Host: ");
            m_client->println(url);
            m_client->println("Connection: close");
            m_client->println();

            readResponse(response);
        }
    }

    return success;

}


void LinkItOneGPRS::readResponse(char * response)
{
    // if there are incoming bytes available
    // from the server, read them and print them:
    uint8_t i = 0;
    
    if (m_connected)
    {
        if (m_client->available())
        {
            response[i++] = m_client->read();
        }

        // if the server's disconnected, stop the m_client:
        if (!m_client->available() && !m_client->connected())
        {
            Serial.println();
            Serial.println("disconnecting.");
            m_client->stop();
        }
    }
}

bool LinkItOneGPRS::isConnected(void) { return m_connected; }
