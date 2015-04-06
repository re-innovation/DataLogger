#ifndef _UPLOAD_MANAGER_H_
#define _UPLOAD_MANAGER_H_

void logLastUpload(char * filename, uint32_t index);
void getFilename(char * buffer, uint8_t maxLength);
uint32_t getIndex(void);

#endif
