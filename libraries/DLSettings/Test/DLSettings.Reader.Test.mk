SRC_FILES += ./DLSettings/DLSettings.cpp
SRC_FILES += ./DLSettings/DLSettings.DataChannels.cpp
SRC_FILES += ./DLSettings/DLSettings.DataChannels.Helper.cpp
SRC_FILES += ./DLSettings/DLSettings.Global.cpp
SRC_FILES += ./DLDataField/DLDataField.cpp
SRC_FILES += ./DLDataField/DLDataField.Numeric.cpp
SRC_FILES += ./DLDataField/DLDataField.Manager.cpp
SRC_FILES += ./DLDataField/DLDataField.Conversion.cpp
SRC_FILES += ./DLUtility/DLUtility.Strings.cpp
SRC_FILES += ./DLUtility/DLUtility.Averager.cpp
SRC_FILES += ./DLUtility/DLUtility.ArrayFunctions.cpp
SRC_FILES += ./DLTest/DLTest.Mock.LocalStorage.cpp

INC_DIRS += -IDLUtility -IDLDataField -IDLLocalStorage

local_setup: ;

local_teardown: ;