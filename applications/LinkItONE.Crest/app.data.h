#ifndef _APP_DATA_H_
#define _APP_DATA_H_

void APP_DATA_Setup(unsigned long msInterval, uint16_t averagerSize, 
	uint16_t storageInterval, uint16_t uploadInterval, char const * const filename);

void APP_DATA_NewData(int32_t data, uint16_t field);

void APP_DATA_WriteHeadersToBuffer(char * buffer, uint8_t bufferLength);

NumericDataField * APP_Data_GetUploadField(uint8_t i);
NumericDataField * APP_Data_GetStorageField(uint8_t i);

uint32_t APP_DATA_GetNumberOfAveragesForStorage(void);
uint32_t APP_DATA_GetNumberOfAveragesForUpload(void);

uint16_t APP_DATA_GetNumberOfFields(void);

void APP_DATA_Tick(void);

bool APP_DATA_StorageDataRemaining(void);
bool APP_DATA_UploadDataRemaining(void);

#endif
