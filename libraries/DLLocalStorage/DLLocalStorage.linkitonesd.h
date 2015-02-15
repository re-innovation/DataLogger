#ifndef _LOCAL_STORAGE_LINKITONESD_H_
#define _LOCAL_STORAGE_LINKITONESD_H_

/*
 * Typedefs
 */

class LinkItOneSD : public LocalStorageInterface
{
    public:
        LinkItOneSD();
        bool FileExists(char * filePath);
        bool DirectoryExists(char * dirPath);
        bool MkDir(char * dirPath);
        void Write(FILE_HANDLE file, char * toWrite);
        uint32_t ReadBytes(FILE_HANDLE file, char * buffer, uint32_t n);
        uint32_t ReadLine(FILE_HANDLE file, char * buffer, uint32_t n);
        FILE_HANDLE OpenFile(char * filename, bool forWrite = false);
        bool EndOfFile(FILE_HANDLE file);
        void CloseFile(FILE_HANDLE file);
};

#endif

