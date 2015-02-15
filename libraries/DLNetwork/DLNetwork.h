#ifndef _NETWORK_IF_H_
#define _NETWORK_IF_H_

#define HTTP_PORT (80)

enum network_interface
{
    NETWORK_INTERFACE_LINKITONE_WIFI,
    NETWORK_INTERFACE_LINKITONE_GPRS
};
typedef enum network_interface NETWORK_INTERFACE;

/*
 * NetworkInterface is a pure abstract class.
 * Each supported interface shall inherit from this base class
 */

class NetworkInterface
{
    public: 
        virtual bool tryConnection(uint8_t timeoutSeconds) = 0;
        virtual bool HTTPGet(char const * const url, char * request, char * response, bool useHTTPS=false) = 0;
};

NetworkInterface * Network_GetNetwork(NETWORK_INTERFACE interface);
        
#endif

