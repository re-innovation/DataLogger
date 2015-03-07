CC = g++

CFLAGS=-Wall -Wextra -Werror

SYMBOLS=-DTEST

SRC_FILES= DLService.thingspeak.example.cpp
SRC_FILES += ../../DLUtility/DLUtility.Strings.cpp
SRC_FILES += ../../DLService/DLService.cpp
SRC_FILES += ../../DLService/DLService.thingspeak.cpp
SRC_FILES += ../../DLSettings/DLSettings.cpp
SRC_FILES += ../../DLDataField/DLDataField.cpp
SRC_FILES += ../../DLHTTP/DLHTTP.RequestBuilder.cpp
SRC_FILES += ../../DLHTTP/DLHTTP.Header.cpp

INC_DIRS = -I../
INC_DIRS += -I../../DLUtility
INC_DIRS += -I../../DLDataField
INC_DIRS += -I../../DLSettings
INC_DIRS += -I../../DLHTTP

SYMBOLS += -D_MAX_FIELDS=6

all:
	$(CC) $(SYMBOLS) $(CFLAGS) $(INC_DIRS) $(SRC_FILES) -o request_builder.exe
	./request_builder.exe