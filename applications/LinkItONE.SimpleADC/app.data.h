#ifndef _APP_DATA_H_
#define _APP_DATA_H_

void APP_Data_Setup(unsigned long storageAveragingInterval, unsigned long uploadAveragingInterval,
	uint16_t averagerSize, uint16_t storageInterval, uint16_t uploadInterval, char const * const filename);

void APP_Data_NewDataArray(int32_t * data);

void APP_Data_WriteHeadersToBuffer(char * buffer, uint8_t bufferLength);

NumericDataField * APP_Data_GetUploadField(uint8_t i);
NumericDataField * APP_Data_GetStorageField(uint8_t i);

void APP_Data_GetUploadData(float * buffer);

uint32_t APP_Data_GetNumberOfAveragesForStorage(void);
uint32_t APP_Data_GetNumberOfAveragesForUpload(void);
uint32_t APP_Data_GetUploadBufferSize(void);

uint16_t APP_Data_GetNumberOfFields(void);

void APP_Data_Tick(void);

uint32_t * APP_Data_GetChannelNumbers(void);

bool APP_Data_StorageDataRemaining(void);
bool APP_Data_UploadDataRemaining(void);

void APP_Data_SetUploadPending(bool pending);
bool APP_Data_UploadIsPending(void);

#endif
