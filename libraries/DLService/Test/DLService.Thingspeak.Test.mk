SRC_FILES += DLUtility/DLUtility.Strings.cpp
SRC_FILES += DLService/DLService.cpp
SRC_FILES += DLSettings/DLSettings.cpp
SRC_FILES += DLDataField/DLDataField.cpp
SRC_FILES += DLHTTP/DLHTTP.RequestBuilder.cpp
SRC_FILES += DLHTTP/DLHTTP.Header.cpp

INC_DIRS += -IDLService
INC_DIRS += -IDLUtility
INC_DIRS += -IDLDataField
INC_DIRS += -IDLSettings
INC_DIRS += -IDLHTTP

SYMBOLS += -D_MAX_FIELDS=6