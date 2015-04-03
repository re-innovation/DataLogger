#include <iostream>

#include "DLDataField.h"

static NumericDataField<float> s_dataFields[] = {
    NumericDataField<float>(VOLTAGE, 60),
    NumericDataField<float>(VOLTAGE, 60),
    NumericDataField<float>(VOLTAGE, 60),
    NumericDataField<float>(VOLTAGE, 60),
    NumericDataField<float>(CURRENT, 60),
    NumericDataField<float>(CURRENT, 60),
    NumericDataField<float>(CURRENT, 60),
    NumericDataField<float>(CURRENT, 60),
    NumericDataField<float>(CURRENT, 60),
    NumericDataField<float>(CURRENT, 60),
    NumericDataField<float>(CURRENT, 60),
    NumericDataField<float>(CURRENT, 60)
};

int main(int argc, char * argv[])
{
	(void)argc; (void)argv;

	char csvHeaders[200];
    DataField_writeHeadersToBuffer(csvHeaders, s_dataFields, 12, 200);

    std::cout << csvHeaders;

    return 0;
}
        