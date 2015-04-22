CC = g++

CFLAGS=-Wall -Wextra -Werror

SYMBOLS=-DTEST

SRC_FILES = DLDataField.Example.cpp
SRC_FILES += ../../../DLDataField/DLDataField.cpp
SRC_FILES += ../../../DLDataField/DLDataField.Manager.cpp
SRC_FILES += ../../../DLUtility/DLUtility.Strings.cpp
SRC_FILES += ../../../DLUtility/DLUtility.ArrayFunctions.cpp

INC_DIRS = -I../../../DLDataField
INC_DIRS += -I../../../DLUtility

all:
	$(CC) $(SYMBOLS) $(CFLAGS) $(INC_DIRS) $(SRC_FILES) -o datafield.exe
	./datafield.exe
