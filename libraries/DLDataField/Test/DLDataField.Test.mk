SRC_FILES += DLDataField/DLDataField.String.cpp DLDataField/DLDataField.Numeric.cpp 
SRC_FILES += DLDataField/DLDataField.Conversion.cpp
SRC_FILES += DLUtility/DLUtility.ArrayFunctions.cpp DLUtility/DLUtility.Strings.cpp
SRC_FILES += DLUtility/DLUtility.Averager.cpp

SRC_FILES += DLSensor/DLSensor.Thermistor.cpp

INC_DIRS += -IDLUtility -IDLSensor

local_setup: ;

local_teardown: ;