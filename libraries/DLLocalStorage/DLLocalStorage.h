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
        virtual bool fileExists(char const * const filePath) = 0;
        virtual bool directoryExists(char const * const dirPath) = 0;
        virtual bool mkDir(char const * const dirPath) = 0;
        virtual void write(FILE_HANDLE file, char const * const toWrite) = 0;
        virtual uint32_t readBytes(FILE_HANDLE file, char * buffer, uint32_t n) = 0;
        virtual uint32_t readLine(FILE_HANDLE file, char * buffer, uint32_t n, bool stripCRLF) = 0;
        virtual FILE_HANDLE openFile(char const * const filename, bool forWrite) = 0;
        virtual void closeFile(FILE_HANDLE file) = 0;
        virtual bool endOfFile(FILE_HANDLE file) = 0;
        virtual void setEcho(bool set) = 0;

    private:
        bool m_echo;
};

LocalStorageInterface * LocalStorage_GetLocalStorageInterface(LOCAL_STORAGE_TYPE storage_type);

#endif