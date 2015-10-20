#ifndef _SMS_IF_H_
#define _SMS_IF_H_

#define _MAX_MESSAGE_LENGTH 140

enum sms_interface
{
	SMS_INTERFACE_LINKITONE
};
typedef enum sms_interface SMS_INTERFACE;

/*
 * SMS Interface class
 * - Defines a standard interface to send SMS messages
 */
class SMSInterface
{
    public:
        virtual bool send(char * pNumber, char * pMessage) = 0;
};

SMSInterface * SMS_GetInterface(SMS_INTERFACE provider);

#endif

