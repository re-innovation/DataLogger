#ifndef _TEST_LOCAL_STORAGE_H_
#define _TEST_LOCAL_STORAGE_H_

class TestStorageInterface : public LocalStorageInterface
{
    public:
        TestStorageInterface();
        bool fileExists(char const * const filePath);
        bool directoryExists(char const * const dirPath);
        bool mkDir(char const * const dirPath);
        void write(FILE_HANDLE file, char const * const toWrite);
        uint32_t readBytes(FILE_HANDLE file, char * buffer, uint32_t n);
        uint32_t readLine(FILE_HANDLE file, char * buffer, uint32_t n, bool stripCRLF);
        FILE_HANDLE openFile(char const * const filename, bool forWrite);
        void closeFile(FILE_HANDLE file);
        bool endOfFile(FILE_HANDLE file);
        void setEcho(bool set);
        void removeFile(char const * const dirPath);

    private:
        bool m_echo;
};

#endif