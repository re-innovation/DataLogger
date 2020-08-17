#ifndef _APP_SMS_H_
#define _APP_SMS_H_

bool APP_SMS_Setup(void);
void APP_SMS_SendGeneralMessage(char * message);
void APP_SMS_SendMaintenanceMessage(char * message);

#endif
