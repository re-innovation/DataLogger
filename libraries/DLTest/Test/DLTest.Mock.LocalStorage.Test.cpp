/*
 * DLTest.LocalStorage.Mock.Test.cpp
 * 
 * Tests the local storage mock harness
 *
 * Author: James Fowkes
 *
 * www.re-innovation.co.uk
 */

/*
 * C++ Library Includes
 */

#include <string.h>
#include <stdint.h>
#include <iostream>
#include <string>
#include <vector>

/*
 * Local Application Includes
 */

#include "DLLocalStorage.h"

/*
 * Unity Test Framework
 */

#include "unity.h"

#define xstr(s) str(s)
#define str(s) #s
#define QUOTED_DL_PATH xstr(DL_PATH)

static LocalStorageInterface * s_testInterface;
static FILE_HANDLE s_handle;

void setUp(void)
{
    s_handle = INVALID_HANDLE;
    s_testInterface = LocalStorage_GetLocalStorageInterface(LOCAL_STORAGE_TYPE(0));
}

void tearDown(void)
{
    s_testInterface->closeFile(s_handle);
}

void test_FileExists_ReturnsTrueForExistingFileFalseForNonExisting(void)
{
    // DL_PATH should be defined by a test makefile and point to the root of the DataLogger folder structure
    TEST_ASSERT_TRUE(s_testInterface->fileExists(QUOTED_DL_PATH "/DLTest/Test/DLTest.Mock.LocalStorage.Test.cpp"));
    TEST_ASSERT_FALSE(s_testInterface->fileExists("This.File.Does.Not.Exist"));
}

void test_FileExists_ReturnsTrueForDirectory(void)
{
    TEST_ASSERT_TRUE(s_testInterface->fileExists(QUOTED_DL_PATH "/DLTest/Test"));
}

void test_DirectoryExists_ReturnsTrueForExistingDirectoryFalseForNonExisting(void)
{
    TEST_ASSERT_TRUE(s_testInterface->directoryExists(QUOTED_DL_PATH "/DLTest/Test"));
    TEST_ASSERT_FALSE(s_testInterface->directoryExists(QUOTED_DL_PATH "/DLNotADirectory/"));
}

void test_DirectoryExists_ReturnsFalseForFile(void)
{
    TEST_ASSERT_FALSE(s_testInterface->directoryExists(QUOTED_DL_PATH "/DLTest/Test/DLTest.Mock.LocalStorage.Test.cpp"));
}

void test_mkdir_CreatesDirectory(void)
{
    TEST_ASSERT_FALSE(s_testInterface->directoryExists(QUOTED_DL_PATH "/DLTest/Test/NewDir"));
    s_testInterface->mkDir(QUOTED_DL_PATH "/DLTest/Test/NewDir");
    TEST_ASSERT_TRUE(s_testInterface->directoryExists(QUOTED_DL_PATH "/DLTest/Test/NewDir"));
}

void test_openFile_CanOpenNewFileForRead(void)
{
    int s_handle = s_testInterface->openFile(QUOTED_DL_PATH "/DLTest/Test/TempForRead", false);
    TEST_ASSERT_NOT_EQUAL(INVALID_HANDLE, s_handle);
    TEST_ASSERT_FALSE(s_testInterface->endOfFile(s_handle));
}

void test_readBytes_CanReadBytesFromOpenFile(void)
{
    int s_handle = s_testInterface->openFile(QUOTED_DL_PATH "/DLTest/Test/TempForRead", false);
    TEST_ASSERT_NOT_EQUAL(INVALID_HANDLE, s_handle);

    char actual[4];
    char expected[] = {'T', 'E', 'S', 'T'};

    s_testInterface->readBytes(s_handle, actual, 4);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, actual, 4);
    TEST_ASSERT_FALSE(s_testInterface->endOfFile(s_handle));
}

void test_readLine_CanReadLineFromOpenFileWithCRLF(void)
{
    int s_handle = s_testInterface->openFile(QUOTED_DL_PATH "/DLTest/Test/TempForRead", false);
    TEST_ASSERT_NOT_EQUAL(INVALID_HANDLE, s_handle);

    char actual[30];
    char expected[] = "TEST FILE CONTENT\r\n";

    // Read length is length of expected plus NULL
    uint32_t count = s_testInterface->readLine(s_handle, actual, 30, false);
    TEST_ASSERT_EQUAL_STRING(expected, actual);
    TEST_ASSERT_EQUAL(strlen(expected) + 1, count);
}

void test_readLine_CanReadLineFromOpenFileWithoutCRLF(void)
{
    int s_handle = s_testInterface->openFile(QUOTED_DL_PATH "/DLTest/Test/TempForRead", false);
    TEST_ASSERT_NOT_EQUAL(INVALID_HANDLE, s_handle);

    char actual[30];
    char expected[] = "TEST FILE CONTENT";

    uint32_t count = s_testInterface->readLine(s_handle, actual, 30, true);
    TEST_ASSERT_EQUAL_STRING(expected, actual);
    TEST_ASSERT_EQUAL(strlen(expected) + 1, count);
}

void test_eof_IsTrueAtEndOfFile(void)
{
    int s_handle = s_testInterface->openFile(QUOTED_DL_PATH "/DLTest/Test/TempForRead", false);
    TEST_ASSERT_FALSE(s_testInterface->endOfFile(s_handle));

    TEST_ASSERT_NOT_EQUAL(INVALID_HANDLE, s_handle);

    char buffer[30];
    s_testInterface->readLine(s_handle, buffer, 30, false);
    s_testInterface->readBytes(s_handle, buffer, 1);

    TEST_ASSERT_TRUE(s_testInterface->endOfFile(s_handle));
}

void test_openFile_CanOpenNewFileForWrite(void)
{
    int s_handle = s_testInterface->openFile(QUOTED_DL_PATH "/DLTest/Test/NewFile", true);
    TEST_ASSERT_NOT_EQUAL(INVALID_HANDLE, s_handle);
}

void test_write_CanWriteBytesToOpenFile(void)
{
    int s_handle = s_testInterface->openFile(QUOTED_DL_PATH "/DLTest/Test/NewFile", true);
    TEST_ASSERT_NOT_EQUAL(INVALID_HANDLE, s_handle);

    char expected[] = "TEST WRITE STRING";
    s_testInterface->write(s_handle, expected);
    s_testInterface->closeFile(s_handle);

    s_handle = s_testInterface->openFile(QUOTED_DL_PATH "/DLTest/Test/NewFile", false);
    char actual[30];
    s_testInterface->readLine(s_handle, actual, 30, true);
    TEST_ASSERT_EQUAL_STRING(expected, actual);
}

int main(void)
{
    UnityBegin("DLTest.LocalStorage.Mock.Test.cpp");

    RUN_TEST(test_FileExists_ReturnsTrueForExistingFileFalseForNonExisting);
    RUN_TEST(test_FileExists_ReturnsTrueForDirectory);
    RUN_TEST(test_DirectoryExists_ReturnsTrueForExistingDirectoryFalseForNonExisting);
    RUN_TEST(test_DirectoryExists_ReturnsFalseForFile);

    RUN_TEST(test_mkdir_CreatesDirectory);

    RUN_TEST(test_openFile_CanOpenNewFileForRead);
    RUN_TEST(test_readBytes_CanReadBytesFromOpenFile);

    RUN_TEST(test_readLine_CanReadLineFromOpenFileWithCRLF);
    RUN_TEST(test_readLine_CanReadLineFromOpenFileWithoutCRLF);
    RUN_TEST(test_eof_IsTrueAtEndOfFile);
    
    RUN_TEST(test_openFile_CanOpenNewFileForWrite);
    RUN_TEST(test_write_CanWriteBytesToOpenFile);
    return 0;
}
