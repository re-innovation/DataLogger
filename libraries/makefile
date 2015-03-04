CC = g++

CFLAGS=-Wall -Wextra -Werror

SRC_FILES=$(UNITY_PATH)/unity.c
INC_DIRS=-I$(UNITY_PATH) -IDLTest

ifdef TEST_MODULE
ifdef TEST_SUBMODULE
SRC_FILES += ./$(TEST_MODULE)/Test/$(TEST_MODULE).$(TEST_SUBMODULE).Test.cpp
SRC_FILES += ./$(TEST_MODULE)/$(TEST_MODULE).$(TEST_SUBMODULE).cpp
TARGET = ./$(TEST_MODULE)/Test/$(TEST_MODULE).$(TEST_SUBMODULE).Test
-include ./$(TEST_MODULE)/Test/$(TEST_MODULE).$(TEST_SUBMODULE).Test.mk
else
SRC_FILES += ./$(TEST_MODULE)/Test/$(TEST_MODULE).Test.cpp
SRC_FILES += ./$(TEST_MODULE)/$(TEST_MODULE).cpp
TARGET = ./$(TEST_MODULE)/Test/$(TEST_MODULE).Test
-include ./$(TEST_MODULE)/Test/$(TEST_MODULE).Test.mk
endif
else
$(error TEST_MODULE is not set)
endif

SYMBOLS=-DTEST

all:
	$(CC) $(CFLAGS) $(INC_DIRS) $(SYMBOLS) $(SRC_FILES) -o $(TARGET)
	$(TARGET)