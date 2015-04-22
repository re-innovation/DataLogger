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

    std::cout << "This is an example of creating data fields and adding them to a datafield manager" << std::endl;

    std::cout << "Adding 4 voltage fields with 60 entries per field... ";
    for (i = 0; i < 4; ++i)
    {
        field = new NumericDataField(VOLTAGE, 60);
        s_dataManager->addField(field);
    }
    std::cout << "the manager count is now " << s_dataManager->count() << "." << std::endl;

    std::cout << "Adding 4 voltage fields with 60 entries per field... ";
    for (i = 0; i < 8; ++i)
    {
        field = new NumericDataField(CURRENT, 60);
        s_dataManager->addField(field);
    }
    std::cout << "the manager count is now " << s_dataManager->count() << "." << std::endl;

    std::cout << "The DataField manager can create comma-separated headers for its fields:" << std::endl;
	
    char csvHeaders[200];
    s_dataManager->writeHeadersToBuffer(csvHeaders, 200);
    
    std::cout << csvHeaders;

    return 0;
}
        