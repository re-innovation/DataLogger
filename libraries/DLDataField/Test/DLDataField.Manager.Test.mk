SRC_FILES += DLTest/DLTest.Mock.Settings.DataChannels.cpp
SRC_FILES += DLDataField/DLDataField.cpp DLDataField/DLDataField.String.cpp
SRC_FILES += DLDataField/DLDataField.Numeric.cpp DLDataField/DLDataField.Conversion.cpp
SRC_FILES += DLUtility/DLUtility.ArrayFunctions.cpp DLUtility/DLUtility.Strings.cpp
SRC_FILES += DLUtility/DLUtility.Averager.cpp

INC_DIRS += -IDLUtility -IDLSettings

local_setup: ;

local_teardown: ;