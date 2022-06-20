/*
 * Mocks:
 * Mocks can be used to isolate the test.Mocked methods have empty implementations. They can be used to control the
 * behaviour of certain methods, like: returning a certain result, calling another method, throwing exceptions.
 *
 * Mocks are a way to isolate your tests by removing the dependencies and replacing them with empty objects.
 * You have full control of them. Mocks can have expectations, you can tell the test that you're expecting a certain
 * method to be call or an exception to be thrown.
 *
 * Test Driven Development (TDD): first you write the tests, then do the actual implementation in
 * your project. However, in TDD, usually the tests fail first, then the implementation is written and then the
 * tests pass.
 *
 * Matchers:
 * Matchers can be used for matching parameters. Special matchers are "_" (anything) or the exact value ("Exactly").
 * There are special matchers for strings. Matchers can be used in assertions on vectors.
 * In tests, we can check if a certain function was called with a certain value.Google mocks has matchers for that :
 * Gt, Ge, Lt, Le, Eq
 * You can combine matchers with Allof()/Anyof()
 * Allof : All the conditions needs to be satisfied
 * Anyof : Only one condition needs to be satisfied
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../MyLibrary/MyLibrary.h"

#include <map>

#include "../MyLibrary/IDatabaseConnection.h"
#include "../MyLibrary/Employee.h"
#include "../MyLibrary/EmployeeManager.h"


// basic mock
class SomeMockedClass : public SomeClass
{
    public:
        MOCK_METHOD(void,someMethod, ()); //(return type, name of method, (parameters) Todo: This is the new version
        //MOCK_METHOD0(someMethod, void()); // This is the legacy version (old one)
};

TEST(SampleMock, BasicTest)
{
    SomeMockedClass smc;
    EXPECT_CALL(smc, someMethod).Times(1);

    smc.someMethod();
}


// Mock Database
class MockDatabaseConnection : public IDatabaseConnection
{
    public:
        explicit MockDatabaseConnection(std::string serverAddress);

        MOCK_METHOD(void, connect, ());
        MOCK_METHOD(void, disconnect, ());
        MOCK_METHOD(float, getSalary, (int), (const));
        MOCK_METHOD(void, updateSalary, (int, float));
        MOCK_METHOD(std::vector<Employee>, getSalariesRange, (float), (const));
        MOCK_METHOD(std::vector<Employee>, getSalariesRange, (float, float), (const));

        void someMemberMethod(std::string param)
        {
            std::cout << "Member method called with param = " << param << "\n";
            throw std::runtime_error("Dummy error!");
        }
};

MockDatabaseConnection::MockDatabaseConnection(std::string serverAddress)  : IDatabaseConnection(serverAddress)
{

}

TEST(TestEmployeeManager, TestConnection)
{
    MockDatabaseConnection dbConnection("dummyConnection");
    EXPECT_CALL(dbConnection, connect());
    EXPECT_CALL(dbConnection, disconnect());

    EmployeeManager employeeManager(&dbConnection);
}


// Times
TEST(TestEmployeeManager, TestUpdateSalary)
{
    const int employeeId = 50;
    const float salary = 6000;
    MockDatabaseConnection dbConnection("dummyConnection");

    EXPECT_CALL(dbConnection, connect());
    EXPECT_CALL(dbConnection, disconnect());
    EXPECT_CALL(dbConnection, updateSalary(employeeId, salary)).Times(1);
    //EXPECT_CALL(dbConnection, updateSalary(testing::_, testing::_)).Times(1); If you dont care with which parameters
    // The function is called

    EmployeeManager employeeManager(&dbConnection);
    employeeManager.setSalary(50, 6000);
}


// Returns
TEST(TestEmployeeManager, TestGetSalary)
{
    const int employeeId = 50;
    const float salary = 6100.0;
    MockDatabaseConnection dbConnection("dummyConnection");

    EXPECT_CALL(dbConnection, connect());
    EXPECT_CALL(dbConnection, disconnect());
    EXPECT_CALL(dbConnection, getSalary(employeeId)).WillOnce(testing::Return(salary));

    EmployeeManager employeeManager(&dbConnection);
    float resSalary = employeeManager.getSalary(employeeId);
    ASSERT_EQ(salary, resSalary);
}


// Invoking actions (3 different ways)
TEST(TestEmployeeManager, TestConnectionError)
{
    MockDatabaseConnection dbConnection("dummyConnection");
    EXPECT_CALL(dbConnection, connect()).WillOnce(testing::Throw(std::runtime_error("Dummy error")));

    ASSERT_THROW(EmployeeManager employeeManager(&dbConnection), std::runtime_error);
}


// Invoking actions by creating an action
ACTION(myThrow)
{
    std::cout << "Throwing an error!\n";
    throw std::runtime_error("Dummy error");
}

TEST(TestEmployeeManager, TestConnectionErrorAction)
{
    MockDatabaseConnection dbConnection("dummyConnection");
    EXPECT_CALL(dbConnection, connect()).WillOnce(myThrow());

    ASSERT_THROW(EmployeeManager employeeManager(&dbConnection), std::runtime_error);
}

// Invoking action by using invoke
void someFreeFunction()
{
    std::cout << "Free function\n";
    throw std::runtime_error("Dummy error");
}

TEST(TestEmployeeManager, TestConnectionErrorInvoke)
{
    MockDatabaseConnection dbConnection("dummyConnection");
    EXPECT_CALL(dbConnection, connect()).WillOnce(testing::Invoke(someFreeFunction));

    ASSERT_THROW(EmployeeManager employeeManager(&dbConnection), std::runtime_error);
}

// Invoking action using invoke but if the method is member of the mock Class
TEST(TestEmployeeManager, TestConnectionErrorInvokeMember)
{
    MockDatabaseConnection dbConnection("dummyConnection");
    auto boundMethod = std::bind(&MockDatabaseConnection::someMemberMethod, &dbConnection, "Some param");
    EXPECT_CALL(dbConnection, connect()).WillOnce(testing::InvokeWithoutArgs(boundMethod));

    ASSERT_THROW(EmployeeManager employeeManager(&dbConnection), std::runtime_error);
}


// Matchers
TEST(TestEmployeeManager, TestGetSalaryInRange)
{
    const int low = 5000, high = 8000;
    std::vector<Employee> returnedVector{Employee{1, 5600, "John"},
                                         Employee{2, 7000, "Jane"},
                                         Employee{3, 6600, "Alex"}};
    MockDatabaseConnection dbConnection("dummyConnection");

    EXPECT_CALL(dbConnection, connect());
    EXPECT_CALL(dbConnection, disconnect());
    EXPECT_CALL(dbConnection, getSalariesRange(low, high)).WillOnce(testing::Return(returnedVector));

    EmployeeManager employeeManager(&dbConnection);
    std::map<std::string, float> resMap =  employeeManager.getSalariesBetween(low, high);

    for(auto & i : resMap) // Equivalent to : for(auto i = resMap.begin(); i != resMap.end(); i++)
    {
        std::cout << i.first << " " << i.second << "\n";
        ASSERT_THAT(i.second, testing::AllOf(testing::Gt(low),testing::Lt(high)));
    }
}


// Assertions on vectors
TEST(VectorTest, ElementAreTest)
{
    std::vector<int> v = generateNumbers(5,3);
    // 0, 1, 2, 0 ,1

    ASSERT_THAT(v, testing::ElementsAre(0,1,2,0,1));
}

TEST(VectorTest, RangeTest)
{
    using namespace testing;
    std::vector<int> v = generateNumbers(5,3);

    ASSERT_THAT(v, Each(AllOf(Ge(0), Lt(3))));
}


// Callback
void realCallback()
{
    std::cout << "Real callback invoked \n";
}

TEST(TestEmployeeManagerCallback, CallbackTest)
{
    MockDatabaseConnection dbConnection("DummyAddress");
    dbConnection.setOnConnect(realCallback);
    dbConnection.connect();
}


// Private methods and static methods


// Main who runs all test suites
int main()
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}