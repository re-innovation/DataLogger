#ifndef _TEST_LOCAL_STORAGE_H_
#define _TEST_LOCAL_STORAGE_H_

class TestNetworkInterface : public NetworkInterface
{
    public:
    	TestNetworkInterface();
        bool tryConnection(uint8_t timeoutSeconds);
        bool sendHTTPRequest(const char * const url, const char * request, char * response, bool useHTTPS=false);
        bool isConnected(void);
};

#endif