#ifndef _APP_SD_STORAGE_H_
#define _APP_SD_STORAGE_H_

void APP_SD_Setup(unsigned long msInterval, uint16_t linesPerFile);
void APP_SD_CreateNewDataFile(void);
void APP_SD_OpenDataFileForToday(void);
void APP_SD_SetNewFilename(void);
void APP_SD_WriteTimestampToOpenFile(void);
void APP_SD_WriteEntryIDToOpenFile(void);
void APP_SD_ReadAllDataFromCurrentFile(char * buffer, uint32_t maxSize);

void APP_SD_Tick(void);

#endif
