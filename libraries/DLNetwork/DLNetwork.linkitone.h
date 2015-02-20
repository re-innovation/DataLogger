#ifndef _THINGSPEAK_H_
#define _THINGSPEAK_H_

/*
 * Typedefs
 */

enum connection_type
{
    LINKIT_ONE_CONN_TYPE_WIFI,
    LINKIT_ONE_CONN_TYPE_GPRS
};
typedef enum connection_type CONNECTION_TYPE;

/*
 * Forward class declarations
 */

class LGPRSClient;

class LinkItOneWiFi : public NetworkInterface
{
    public:
        LinkItOneWiFi();
        ~LinkItOneWiFi();
        bool tryConnection(uint8_t timeoutSeconds);
        bool HTTPGet(char const * const url, char * request, char * response, bool useHTTPS=false);
        bool isConnected(void);
        
    private:
        bool m_connected;
};

class LinkItOneGPRS : public NetworkInterface
{
    public:
        LinkItOneGPRS(char * apn, char * username, char * password);
        ~LinkItOneGPRS();
        bool tryConnection(uint8_t timeoutSeconds);
        bool HTTPGet(char const * const url, char * request, char * response, bool useHTTPS=false);
        bool isConnected(void);

    private:
        char * m_pAPN;
        char * m_pUser;
        char * m_pPwd;
        bool m_connected;
        LGPRSClient * m_client;
        void readResponse(char *);
};

#endif

