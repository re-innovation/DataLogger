#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include <string.h>

#include "app.serial-request-data.h"

class SerialRequestDataTest : public CppUnit::TestFixture  {

    CPPUNIT_TEST_SUITE(SerialRequestDataTest);
    CPPUNIT_TEST(TestDataFormatterReturnsFalseForNULLPointersOrZeroLengthData);
    CPPUNIT_TEST(TestDataFormatterCorrectlyFormatsPartialDataArray);
    CPPUNIT_TEST(TestDataFormatterCorrectlyFormatsFullDataArray);
    CPPUNIT_TEST(TestDataFormatterCorrectlyFormatsTemperatureArray);
    CPPUNIT_TEST_SUITE_END();

    void TestDataFormatterReturnsFalseForNULLPointersOrZeroLengthData()
    {
        char buffer[128] = "";
        float data[] = {0.0f};

        bool actual = APP_SerialRequestData_FormatArray(NULL, 0, NULL, 0);
        CPPUNIT_ASSERT(!actual);
        CPPUNIT_ASSERT_EQUAL(0, (int)strlen(buffer));

        actual = APP_SerialRequestData_FormatArray(data, 0, NULL, 0);
        CPPUNIT_ASSERT_EQUAL(0, (int)strlen(buffer));
        CPPUNIT_ASSERT(!actual);

        actual = APP_SerialRequestData_FormatArray(NULL, 0, buffer, 0);
        CPPUNIT_ASSERT_EQUAL(0, (int)strlen(buffer));
        CPPUNIT_ASSERT(!actual);
    }

    void TestDataFormatterCorrectlyFormatsPartialDataArray()
    {
        char buffer[128] = "";
        float data[] = {0.0f, 1.1f, 2.2f, 3.3f};

        bool actual = APP_SerialRequestData_FormatArray(data, 4, buffer, 128);
        CPPUNIT_ASSERT_EQUAL(std::string("s0.0,1.1,2.2,3.3t"), std::string(buffer));
        CPPUNIT_ASSERT(actual); 
    }

    void TestDataFormatterCorrectlyFormatsFullDataArray()
    {
        char buffer[128] = "";
        float data[] = {0.0f,  1.1f, 2.2f,  3.3f,  4.4f, 5.5f, 6.6f,  7.7f, 8.8f,  9.9f,  10.1f, 11.11f};

        bool actual = APP_SerialRequestData_FormatArray(data, 12, buffer, 128);
        CPPUNIT_ASSERT_EQUAL(std::string("s0.0,1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8,9.9,10.1,11.1t"), std::string(buffer));
        CPPUNIT_ASSERT(actual);
    }

    void TestDataFormatterCorrectlyFormatsTemperatureArray()
    {
        char buffer[128] = "";
        float data[] = {-25.5f, 0.0f, 25.0f};
    
        bool actual = APP_SerialRequestData_FormatArray(data, 3, buffer, 128);
        CPPUNIT_ASSERT_EQUAL(std::string("s-25.5,0.0,25.0t"), std::string(buffer));
        CPPUNIT_ASSERT(actual);
    }

public:
    void setUp()
    {

    }
};

int main ()
{
    CppUnit::TextUi::TestRunner runner;
    
    CPPUNIT_TEST_SUITE_REGISTRATION( SerialRequestDataTest );

    CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();

    runner.addTest( registry.makeTest() );
    runner.run();

    return 0;
}
