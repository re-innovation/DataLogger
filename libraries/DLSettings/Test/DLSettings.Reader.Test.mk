SRC_FILES += ./DLSettings/DLSettings.Global.cpp
SRC_FILES += ./DLSettings/DLSettings.Reader.Errors.cpp

SRC_FILES += ./DLUtility/DLUtility.Strings.cpp
SRC_FILES += ./DLTest/DLTest.Mock.LocalStorage.cpp

INC_DIRS += -IDLUtility -IDLLocalStorage

local_setup: ;

local_teardown: ;