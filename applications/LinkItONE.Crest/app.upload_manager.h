#ifndef _UPLOAD_MANAGER_H_
#define _UPLOAD_MANAGER_H_

void UploadMgr_logLastUpload(char * filename, uint32_t index);
void UploadMgr_getFilename(char * buffer, uint8_t maxLength);
uint32_t UploadMgr_getIndex(void);

#endif
