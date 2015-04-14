INC_DIRS += -IDLLocalStorage
INC_DIRS += -IDLUtility

local_setup:
	# Remove test directory
	rm -rf ./DLTest/Test/NewDir

	# Remove test files
	rm -f ./DLTest/Test/NewFile

	# Create a file for reading
	printf "TEST FILE CONTENT\r\n" > ./DLTest/Test/TempForRead

local_teardown:
	rm -f ./DLTest/Test/TempForRead
	rm -f ./DLTest/Test/NewFile
	rm -rf ./DLTest/Test/NewDir