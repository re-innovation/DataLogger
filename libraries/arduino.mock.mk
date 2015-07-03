CC = g++

CFLAGS=-Wall -Wextra -Werror

INC_DIRS = -IDLCSV -IDLDataField -IDLFilename -IDLGPS -IDLHTTP -IDLLocalStorage -IDLLocation
INC_DIRS += -IDLNetwork -IDLSecurity -IDLSensor -IDLService -IDLSettings -IDLSMS -IDLPlatform
INC_DIRS += -IDLTest  -IDLTime -IDLUtility -ITaskAction

DIR = $(dir $(INO_FILE))
MODIFIED_INO = $(INO_FILE).modified
TARGET = $(INO_FILE).mocked

SRC_FILES = DLTest/DLTest.Mock.LocalStorage.cpp DLTest/DLTest.Mock.Serial.cpp DLTest/DLTest.Mock.delay.cpp
SRC_FILES += DLTest/DLTest.Mock.random.cpp DLTest/DLTest.Mock.arduino.cpp DLTest/DLTest.Mock.Sensor.ADS1x1x.cpp
SRC_FILES += DLTest/DLTest.Mock.Location.cpp DLTest/DLTest.Mock.Time.cpp DLTest/DLTest.Mock.GPS.cpp
SRC_FILES += DLTest/DLTest.Mock.Network.cpp DLTest/DLTest.Mock.Sensor.LinkItONE.cpp TaskAction/TaskAction.cpp
SRC_FILES += DLTest/DLTest.Mock.Battery.cpp DLTest/DLTest.Mock.SMS.LinkItONE.cpp

SYMBOLS = -g -DMOCK -DTEST

-include $(DIR)/mock.mk

OBJDEPS=$(SRC_FILES:.cpp=.o)

all: clean $(TARGET)
	
$(TARGET): $(OBJDEPS)
	echo "#include <DLTest.Mock.arduino.h>" > $(MODIFIED_INO)
	cat $(INO_FILE) >> $(MODIFIED_INO)
	$(CC) $(CFLAGS) $(INC_DIRS) $(SYMBOLS) -c -x c++ $(MODIFIED_INO) -o $(MODIFIED_INO).o
	$(CC) $(CFLAGS) $(INC_DIRS) $(SYMBOLS) $(OBJDEPS) $(MODIFIED_INO).o -o $(TARGET)
	cp $(TARGET) $(TARGET).debug
	$(TARGET)
	rm $(MODIFIED_INO)

.cpp.o:
	$(CC) $(CFLAGS) $(INC_DIRS) $(SYMBOLS) -c $< -o $@

clean:
	rm -f $(TARGET)
	rm -f $(OBJDEPS)
