#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include "app.serial-interface.h"

class SerialInterfaceTest : public CppUnit::TestFixture  {

    CPPUNIT_TEST_SUITE(SerialInterfaceTest);
    CPPUNIT_TEST(TestServiceRequestReturnsInvalidForEmptyRequest);
    CPPUNIT_TEST(TestServiceRequestReturnsInvalidForTooLongRequest);
    CPPUNIT_TEST(TestServiceRequestReturnsInvalidForRequestsNotStartingWithS);
    CPPUNIT_TEST(TestServiceRequestReturnsCorrectRequestIDForValidRequests);
    CPPUNIT_TEST(TestServiceRequestIgnoresNonPrintables);
    CPPUNIT_TEST_SUITE_END();

    bool callback_invoked;
    int request_number;
    void TestServiceRequestReturnsInvalidForEmptyRequest()
    {
        APP_SerialInterface_HandleChar('?');
        CPPUNIT_ASSERT(callback_invoked);
        CPPUNIT_ASSERT_EQUAL(INVALID_REQUEST, request_number);
    }

    void TestServiceRequestReturnsInvalidForTooLongRequest()
    {
        APP_SerialInterface_HandleChar('s');
        APP_SerialInterface_HandleChar('0');
        APP_SerialInterface_HandleChar('0');
        APP_SerialInterface_HandleChar('0');
        APP_SerialInterface_HandleChar('?');
        
        CPPUNIT_ASSERT(callback_invoked);
        CPPUNIT_ASSERT_EQUAL(INVALID_REQUEST, request_number);
    }

    void TestServiceRequestReturnsInvalidForRequestsNotStartingWithS()
    {
        APP_SerialInterface_HandleChar('0');
        APP_SerialInterface_HandleChar('1');
        APP_SerialInterface_HandleChar('?');
        
        CPPUNIT_ASSERT(callback_invoked);
        CPPUNIT_ASSERT_EQUAL(INVALID_REQUEST, request_number);
    }

    void TestServiceRequestReturnsCorrectRequestIDForValidRequests()
    {
        APP_SerialInterface_HandleChar('s');
        APP_SerialInterface_HandleChar('0');
        APP_SerialInterface_HandleChar('0');
        APP_SerialInterface_HandleChar('?');
        CPPUNIT_ASSERT(callback_invoked);   
        CPPUNIT_ASSERT_EQUAL(0, request_number);
    
        callback_invoked = false;
        APP_SerialInterface_HandleChar('s');
        APP_SerialInterface_HandleChar('0');
        APP_SerialInterface_HandleChar('1');
        APP_SerialInterface_HandleChar('?');   
        CPPUNIT_ASSERT(callback_invoked);   
        CPPUNIT_ASSERT_EQUAL(1, request_number);
    
        callback_invoked = false;
        APP_SerialInterface_HandleChar('s');
        APP_SerialInterface_HandleChar('1');
        APP_SerialInterface_HandleChar('0');
        APP_SerialInterface_HandleChar('?');   
        CPPUNIT_ASSERT(callback_invoked);   
        CPPUNIT_ASSERT_EQUAL(10, request_number);
    
        callback_invoked = false;
        APP_SerialInterface_HandleChar('s');
        APP_SerialInterface_HandleChar('9');
        APP_SerialInterface_HandleChar('9');
        APP_SerialInterface_HandleChar('?');   
        CPPUNIT_ASSERT(callback_invoked);   
        CPPUNIT_ASSERT_EQUAL(99, request_number);
    }

    void TestServiceRequestIgnoresNonPrintables()
    {
        APP_SerialInterface_HandleChar('\r');
        APP_SerialInterface_HandleChar('\n');
        APP_SerialInterface_HandleChar('s');
        APP_SerialInterface_HandleChar('5');
        APP_SerialInterface_HandleChar('0');
        APP_SerialInterface_HandleChar('?');   
        CPPUNIT_ASSERT(callback_invoked);   
        CPPUNIT_ASSERT_EQUAL(50, request_number);      
    }

    static void on_handler_callback(int new_request_number)
    {
        _this->request_number = new_request_number;
        _this->callback_invoked = true;
    }

    public:

    static SerialInterfaceTest * _this;

    void setUp()
    {
        APP_SerialInterface_Setup(on_handler_callback);
        callback_invoked = false;
        SerialInterfaceTest::_this = this;
    }
};

SerialInterfaceTest * SerialInterfaceTest::_this =  NULL;

int main ()
{
    CppUnit::TextUi::TestRunner runner;
    
    CPPUNIT_TEST_SUITE_REGISTRATION( SerialInterfaceTest );

    CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();

    runner.addTest( registry.makeTest() );
    runner.run();

    return 0;
}
