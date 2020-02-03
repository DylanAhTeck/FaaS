#include "kvstore.grpc.pb.h"
#include "../googletest/googletest/include/gtest/gtest.h"
#include <grpcpp/grpcpp.h>
#include "kvstore_mock.grpc.pb.h"

#include "../grpc/test/core/util/port.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReaderWriter;
using grpc::Status;

using kvstore::KeyValueStore;

//LOOK AT https://github.com/grpc/grpc/blob/master/test/cpp/end2end/mock_test.cc FOR REFERENCE

// The fixture for testing class Foo.
class KvstoreServerTests : public ::testing::Test
{

protected:
    // You can remove any or all of the following functions if their bodies would
    // be empty.

    KvstoreServerTests()
    {
        // You can do set-up work for each test here.
    }

    ~KvstoreServerTests() override
    {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    void SetUp() override
    {
        // Code here will be called immediately after the constructor (right
        // before each test).
    }

    // Code here will be called immediately after each test (right
    // before the destructor).
    void TearDown() override {}

    // Class members declared here can be used by all tests in the test suite
    // for Foo.
};

// Tests that the Foo::Bar() method does Abc.
TEST_F(KvstoreServerTests, TestPut)
{
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}