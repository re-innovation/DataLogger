#ifndef _DLSMS_MESSAGING_H_
#define _DLSMS_MESSAGING_H_

void SMS_Setup(SMS_INTERFACE interface_type);
void SMS_SendGeneralMessage(char * message);
void SMS_SendMaintenanceMessage(char * message);

#endif
