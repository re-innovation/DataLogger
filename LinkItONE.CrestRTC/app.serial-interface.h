#ifndef _APP_SERIAL_INTERFACE_H_
#define _APP_SERIAL_INTERFACE_H_

#define INVALID_REQUEST (-1)

typedef void (*ON_SERIAL_REQUEST_CB)(int);

void APP_SerialInterface_Setup(ON_SERIAL_REQUEST_CB handler);
void APP_SerialInterface_HandleChar(char c);
void APP_SerialInterface_SetDebug(bool on);

#endif
