#ifndef _APP_DATA_H_
#define _APP_DATA_H_

void APP_DATA_Setup(unsigned long msInterval,
    uint16_t fieldCount, uint16_t averagerSize, uint16_t dataFieldBufferSize, FIELD_TYPE fieldTypes[]);

void APP_DATA_NewData(uint16_t data, uint16_t field);

void APP_DATA_WriteHeadersToBuffer(char * buffer, uint8_t bufferLength);

NumericDataField * APP_Data_GetField(uint8_t i);
uint16_t APP_DATA_GetNumberOfFields(void);

void APP_DATA_Tick(void);

#endif
