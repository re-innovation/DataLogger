CC = g++

CFLAGS=-Wall -Wextra -Werror

SYMBOLS=-DTEST

TARGET = DLSettings.Reader.Example
SRC_FILES= $(TARGET).cpp
SRC_FILES += ../../../DLSettings/DLSettings.cpp
SRC_FILES += ../../../DLSettings/DLSettings.Reader.cpp

INC_DIRS = -I../../
INC_DIRS += -I../../../DLSettings

all:
	$(CC) $(SYMBOLS) $(CFLAGS) $(INC_DIRS) $(SRC_FILES) -o $(TARGET).exe
	./$(TARGET).exe