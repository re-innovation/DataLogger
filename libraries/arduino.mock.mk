CC = g++

CFLAGS=-Wall -Wextra -Werror

INC_DIRS = -IDLLocalStorage -IDLTest -IDLUtility

SRC_FILES = DLTest/DLTest.Mock.LocalStorage.cpp DLTest/DLTest.Mock.Serial.cpp DLTest/DLTest.Mock.delay.cpp DLTest/DLTest.Mock.random.cpp DLTest/DLTest.Mock.arduino.cpp

TARGET = $(INO_FILE).mocked

all:
	$(CC) $(CFLAGS) $(INC_DIRS) $(SYMBOLS) $(SRC_FILES) -x c++ $(INO_FILE) -o $(TARGET)
	$(TARGET)
