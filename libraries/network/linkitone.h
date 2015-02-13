#ifndef _THINGSPEAK_H_
#define _THINGSPEAK_H_

enum connection_type
{
    LINKIT_ONE_CONN_TYPE_WIFI,
    LINKIT_ONE_CONN_TYPE_GPRS
};
typedef enum connection_type CONNECTION_TYPE;

class LinkItOneWiFi : public NetworkInterface
{
    public:
        LinkItOneWiFi();
        ~LinkItOneWiFi();

        bool tryConnection(void);    
        bool HTTPGet(char * url, char * request, bool useHTTPS);
};

class LinkItOneGPRS : public NetworkInterface
{
    public:
        LinkItOneGPRS(char * apn, char * username, char * password);
        ~LinkItOneGPRS();
    
        bool tryConnection(void);
        bool HTTPGet(char * url, char * request, bool useHTTPS);
        
    private:
        char * m_pAPN;
        char * m_pUser;
        char * m_pPwd;
        LGPRSClient m_client;
};

#endif

