#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include "app.serial-interface.h"

class SerialInterfaceTest : public CppUnit::TestFixture  {

    CPPUNIT_TEST_SUITE(SerialInterfaceTest);
    CPPUNIT_TEST(TestServiceRequestReturnsInvalidForNULLRequest);
    CPPUNIT_TEST(TestServiceRequestReturnsInvalidForEmptyRequest);
    CPPUNIT_TEST(TestServiceRequestReturnsInvalidForTooLongRequest);
    CPPUNIT_TEST(TestServiceRequestReturnsInvalidForRequestsNotEndingWithQuestionMark);
    CPPUNIT_TEST(TestServiceRequestReturnsInvalidForRequestsNotStartingWithS);
    CPPUNIT_TEST(TestServiceRequestReturnsCorrectRequestID);
    CPPUNIT_TEST_SUITE_END();

    void TestServiceRequestReturnsInvalidForNULLRequest()
    {
        CPPUNIT_ASSERT_EQUAL(-1, APP_SerialInterface_HandleRequest(NULL));
    }

    void TestServiceRequestReturnsInvalidForEmptyRequest()
    {
        CPPUNIT_ASSERT_EQUAL(-1, APP_SerialInterface_HandleRequest(""));
    }

    void TestServiceRequestReturnsInvalidForTooLongRequest()
    {
        CPPUNIT_ASSERT_EQUAL(-1, APP_SerialInterface_HandleRequest("s000?"));
    }

    void TestServiceRequestReturnsInvalidForRequestsNotEndingWithQuestionMark()
    {
        CPPUNIT_ASSERT_EQUAL(-1, APP_SerialInterface_HandleRequest("s01"));
    }

    void TestServiceRequestReturnsInvalidForRequestsNotStartingWithS()
    {
        CPPUNIT_ASSERT_EQUAL(-1, APP_SerialInterface_HandleRequest("01?"));
    }

    void TestServiceRequestReturnsCorrectRequestID()
    {
        CPPUNIT_ASSERT_EQUAL(0, APP_SerialInterface_HandleRequest("s00?"));
        CPPUNIT_ASSERT_EQUAL(1, APP_SerialInterface_HandleRequest("s01?"));
        CPPUNIT_ASSERT_EQUAL(10, APP_SerialInterface_HandleRequest("s10?"));
        CPPUNIT_ASSERT_EQUAL(99, APP_SerialInterface_HandleRequest("s99?"));
    }
};

int main ()
{
    CppUnit::TextUi::TestRunner runner;
    
    CPPUNIT_TEST_SUITE_REGISTRATION( SerialInterfaceTest );

    CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();

    runner.addTest( registry.makeTest() );
    runner.run();

    return 0;
}
