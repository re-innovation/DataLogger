#ifndef _APP_DATA_H_
#define _APP_DATA_H_

void APP_DATA_Setup(unsigned long storageAveragingInterval, unsigned long uploadAveragingInterval,
	uint16_t averagerSize, uint16_t storageInterval, uint16_t uploadInterval, char const * const filename);

void APP_DATA_NewDataArray(int32_t * data);

void APP_DATA_WriteHeadersToBuffer(char * buffer, uint8_t bufferLength);

NumericDataField * APP_DATA_GetUploadField(uint8_t i);
NumericDataField * APP_DATA_GetStorageField(uint8_t i);

void APP_DATA_GetUploadData(float * buffer);

uint32_t APP_DATA_GetNumberOfAveragesForStorage(void);
uint32_t APP_DATA_GetNumberOfAveragesForUpload(void);
uint32_t APP_DATA_GetUploadBufferSize(void);

uint16_t APP_DATA_GetNumberOfFields(void);

void APP_DATA_Tick(void);

uint32_t * APP_DATA_GetChannelNumbers(void);

bool APP_DATA_StorageDataRemaining(void);
bool APP_DATA_UploadDataRemaining(void);

bool APP_DATA_UploadIsPending(void);

#endif
