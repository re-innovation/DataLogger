CC = g++

CFLAGS=-Wall -Wextra -Werror

SRC_FILES=$(UNITY_PATH)/unity.c
INC_DIRS=-I$(UNITY_PATH) -IDLTest
SYMBOLS=-DTEST -DDL_PATH=\".\"

ifdef TEST_MODULE
ifdef TEST_SUBMODULE
SRC_FILES += ./$(TEST_MODULE)/Test/$(TEST_MODULE).$(TEST_SUBMODULE).Test.cpp
SRC_FILES += ./$(TEST_MODULE)/$(TEST_MODULE).$(TEST_SUBMODULE).cpp
TARGET = ./$(TEST_MODULE)/Test/$(TEST_MODULE).$(TEST_SUBMODULE).Test
TEST_MAKEFILE = ./$(TEST_MODULE)/Test/$(TEST_MODULE).$(TEST_SUBMODULE).Test.mk
-include ./$(TEST_MODULE)/Test/$(TEST_MODULE).$(TEST_SUBMODULE).Test.mk
else
SRC_FILES += ./$(TEST_MODULE)/Test/$(TEST_MODULE).Test.cpp
SRC_FILES += ./$(TEST_MODULE)/$(TEST_MODULE).cpp
TARGET = ./$(TEST_MODULE)/Test/$(TEST_MODULE).Test
TEST_MAKEFILE = ./$(TEST_MODULE)/Test/$(TEST_MODULE).Test.mk
endif
else
$(error TEST_MODULE is not set)
endif

-include TEST_MAKEFILE

all:
	make -s -f $(TEST_MAKEFILE) local_setup
	$(CC) $(CFLAGS) $(INC_DIRS) $(SYMBOLS) $(SRC_FILES) -o $(TARGET)
	@echo "------------RUNNING TESTS------------"
	@echo
	$(TARGET)
	@echo
	@echo
	@echo "------------TESTS COMPLETE-----------"
	make -s -f $(TEST_MAKEFILE) local_teardown

.DEFAULT_GOAL := all