#ifndef _APP_DATA_H_
#define _APP_DATA_H_

void APP_Data_Setup(unsigned long msStorageAverageInterval,	uint16_t averagerSize, uint16_t storageInterval, char const * const filename);

void APP_Data_NewData(int32_t data, uint16_t field);

void APP_Data_WriteHeadersToBuffer(char * buffer, uint8_t bufferLength);

NumericDataField * APP_Data_GetField(uint8_t i);
uint16_t APP_Data_GetNumberOfFields(void);

void APP_Data_Tick(void);

#endif
