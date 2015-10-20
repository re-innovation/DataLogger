CC = g++

CFLAGS=-Wall -Wextra -Werror

SYMBOLS=-DTEST

TARGET = DLSettings.Reader.Example
SRC_FILES= $(TARGET).cpp

SRC_FILES += ../../../DLSettings/DLSettings.cpp
SRC_FILES += ../../../DLSettings/DLSettings.Reader.cpp
SRC_FILES += ../../../DLSettings/DLSettings.Global.cpp
SRC_FILES += ../../../DLSettings/DLSettings.DataChannels.cpp
SRC_FILES += ../../../DLSettings/DLSettings.DataChannels.Helper.cpp

SRC_FILES += ../../../DLDataField/DLDataField.cpp
SRC_FILES += ../../../DLDataField/DLDataField.Numeric.cpp
SRC_FILES += ../../../DLDataField/DLDataField.Manager.cpp
SRC_FILES += ../../../DLDataField/DLDataField.Conversion.cpp

SRC_FILES += ../../../DLUtility/DLUtility.Strings.cpp
SRC_FILES += ../../../DLUtility/DLUtility.ArrayFunctions.cpp
SRC_FILES += ../../../DLUtility/DLUtility.Averager.cpp
SRC_FILES += ../../../DLTest/DLTest.Mock.LocalStorage.cpp

INC_DIRS = -I../../
INC_DIRS += -I../../../DLSettings
INC_DIRS += -I../../../DLDataField
INC_DIRS += -I../../../DLUtility
INC_DIRS += -I../../../DLLocalStorage
INC_DIRS += -I../../../DLTest

all:
	$(CC) $(SYMBOLS) $(CFLAGS) $(INC_DIRS) $(SRC_FILES) -o $(TARGET).exe
	./$(TARGET).exe