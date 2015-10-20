#ifndef _DLSMS_LINKINTONE_H_
#define _DLSMS_LINKINTONE_H_

class LinkItOneSMSInterface : public SMSInterface
{
    public:
        LinkItOneSMSInterface();
        ~LinkItOneSMSInterface();
        bool send(char * pNumber, char * pMessage);
};

#endif
