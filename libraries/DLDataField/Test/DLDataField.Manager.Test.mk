SRC_FILES += DLDataField/DLDataField.cpp DLDataField/DLDataField.String.cpp
SRC_FILES += DLDataField/DLDataField.Numeric.cpp DLDataField/DLDataField.Conversion.cpp
SRC_FILES += DLUtility/DLUtility.ArrayFunctions.cpp DLUtility/DLUtility.Strings.cpp
SRC_FILES += DLUtility/DLUtility.Averager.cpp

SRC_FILES += DLSensor/DLSensor.Thermistor.cpp

SRC_FILES += DLSettings/DLSettings.DataChannels.cpp DLSettings/DLSettings.DataChannels.Helper.cpp

SRC_FILES += DLPlatform/DLPlatform.cpp

INC_DIRS += -IDLUtility -IDLSettings -IDLSensor -IDLSettings -IDLPlatform

local_setup: ;

local_teardown: ;