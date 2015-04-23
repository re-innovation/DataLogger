CC = g++

CFLAGS=-Wall -Wextra -Werror

INC_DIRS = -IDLCSV -IDLDataField -IDLFilename -IDLGPS -IDLHTTP -IDLLocalStorage -IDLLocation
INC_DIRS += -IDLNetwork -IDLSecurity -IDLSensor -IDLService -IDLSettings -IDLSMS 
INC_DIRS += -IDLTest  -IDLTime -IDLUtility -ITaskAction

DIR = $(dir $(INO_FILE))
MODIFIED_INO = $(INO_FILE).modified
TARGET = $(INO_FILE).mocked

APP_FILES := $(wildcard $(DIR)*.cpp)
SRC_FILES = DLTest/DLTest.Mock.LocalStorage.cpp DLTest/DLTest.Mock.Serial.cpp DLTest/DLTest.Mock.delay.cpp
SRC_FILES += DLTest/DLTest.Mock.random.cpp DLTest/DLTest.Mock.arduino.cpp DLTest/DLTest.Mock.Sensor.ADS1x1x.cpp
SRC_FILES += DLTest/DLTest.Mock.Location.cpp DLTest/DLTest.Mock.Time.cpp TaskAction/TaskAction.cpp

SYMBOLS = -DMOCK

-include $(DIR)/mock.mk

all:
	echo "#include <DLTest.Mock.arduino.h>" > $(MODIFIED_INO)
	cat $(INO_FILE) >> $(MODIFIED_INO)
	$(CC) -g $(CFLAGS) $(INC_DIRS) $(SYMBOLS) $(SRC_FILES) $(APP_FILES) -x c++ $(MODIFIED_INO) -o $(TARGET)
	$(TARGET)
	rm $(MODIFIED_INO)
