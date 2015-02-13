#ifndef _NETWORK_IF_H_
#define _NETWORK_IF_H_

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
        virtual bool HTTPGet(char * url, char * request, bool useHTTPS) = 0;
};

NetworkInterface * GetInterface(NETWORK_INTERFACE interface);
        
#endif

