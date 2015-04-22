#include <iostream>

#include "DLDataField.h"
#include "DLDataField.Manager.h"

static DataFieldManager * s_dataManager;

int main(int argc, char * argv[])
{
	(void)argc; (void)argv;

    s_dataManager = new DataFieldManager();

    uint8_t i;
    DataField * field;

    for (i = 0; i < 4; ++i)
    {
        field = new NumericDataField<float>(VOLTAGE, 60);
        s_dataManager->addField(field);
    }
    for (i = 0; i < 8; ++i)
    {
        field = new NumericDataField<float>(CURRENT, 60);
        s_dataManager->addField(field);
    }

	char csvHeaders[200];
    DataField_writeHeadersToBuffer(csvHeaders, s_dataManager->getFields(), 12, 200);

    std::cout << csvHeaders;

    return 0;
}
        