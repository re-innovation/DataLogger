#include <iostream>

#include "DLUtility.Averager.h"
#include "DLDataField.h"
#include "DLDataField.Manager.h"

static DataFieldManager * s_dataManager;

static VOLTAGECHANNEL s_voltageChannelSettings = {
    .mvPerBit = 0.125f,
    .R1 = 200000.0f,
    .R2= 10000.0f,
};

static CURRENTCHANNEL s_currentChannelSettings = {
    .mvPerBit = 0.125f,
    .offset = 60.0f,
    .mvPerAmp = 600.0f,
};

int main(int argc, char * argv[])
{
	(void)argc; (void)argv;

    std::cout << "This is an example of creating data fields and adding them to a datafield manager" << std::endl;
    std::cout << "Creating a manager with a 60-average buffer and 10 datapoints per average" << std::endl;

    s_dataManager = new DataFieldManager(60, 10);

    uint8_t i, j;
    NumericDataField * field;

    std::cout << "Adding 4 voltage fields... ";
    for (i = 0; i < 4; ++i)
    {
        field = new NumericDataField(VOLTAGE, &s_voltageChannelSettings);
        s_dataManager->addField(field);
    }
    std::cout << "the manager count is now " << (int)s_dataManager->count() << "." << std::endl;

    std::cout << "Adding 4 current fields... ";
    for (i = 0; i < 8; ++i)
    {
        field = new NumericDataField(CURRENT, &s_currentChannelSettings);
        s_dataManager->addField(field);
    }
    std::cout << "the manager count is now " << (int)s_dataManager->count() << "." << std::endl;

    std::cout << "The DataField manager can create comma-separated headers for its fields:" << std::endl;
	
    char csvHeaders[200];
    s_dataManager->writeHeadersToBuffer(csvHeaders, 200);
    
    std::cout << csvHeaders << std::endl;

    std::cout << "The DataField manager can return its fields for adding data:" << std::endl;

    for (i = 0; i < 12; ++i)
    {
        for (j = 0; j < 10; j++)
        {
            field = (NumericDataField*)s_dataManager->getField(i);
            field->storeData(512);
        }

        std::cout << "Field" << (int)i << " Data: Raw=" << field->getRawData(0);
        std::cout << ", Converted=" << field->getConvData(0) << std::endl;
    }

    for (i = 0; i < 12; ++i)
    {
        for (j = 0; j < 10; j++)
        {
            field = (NumericDataField*)s_dataManager->getField(i);
            field->storeData(1024);
        }

        std::cout << "Field" << (int)i << " Data: Raw=" << field->getRawData(1);
        std::cout << ", Converted=" << field->getConvData(1) << std::endl;
    }
    
    return 0;
}
        