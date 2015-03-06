#ifndef _LOCAL_STORAGE_H_
#define _LOCAL_STORAGE_H_


enum local_storage_type
{
    LINKITONE_SD_CARD
};
typedef enum local_storage_type LOCAL_STORAGE_TYPE;

/*
 * Storage Interface class
 * - Defines a standard interface to read/write files on local storage media
 */
#define INVALID_HANDLE ((FILE_HANDLE)(-1))
typedef int8_t FILE_HANDLE;

class LocalStorageInterface
{
    public:
        virtual bool FileExists(char * filePath) = 0;
        virtual bool DirectoryExists(char * dirPath) = 0;
        virtual bool MkDir(char * dirPath) = 0;
        virtual void Write(FILE_HANDLE file, char * toWrite) = 0;
        virtual uint32_t ReadBytes(FILE_HANDLE file, char * buffer, uint32_t n) = 0;
        virtual uint32_t ReadLine(FILE_HANDLE file, char * buffer, uint32_t n) = 0;
        virtual FILE_HANDLE OpenFile(char * filename, bool forWrite) = 0;
        virtual void CloseFile(FILE_HANDLE file) = 0;
        virtual bool EndOfFile(FILE_HANDLE file) = 0;
};

LocalStorageInterface * LocalStorage_GetLocalStorageInterface(LOCAL_STORAGE_TYPE stroage_type);

#endif