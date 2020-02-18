#include "database.h"

class KvstoreServerTests : public ::testing::Test
{

    // You can remove any or all of the following functions if their bodies would
    // be empty.
public:
    KvstoreServerTests()
    {
    }

    ~KvstoreServerTests() override
    {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    void SetUp() override
    {
        //Set up Server
        std::string server_address("0.0.0.0:50001");
        //KeyValueStoreServiceImpl service;

        ServerBuilder builder;
        // Listen on the given address without any authentication mechanism.
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
        // Register "service" as the instance through which we'll communicate with
        // clients. In this case, it corresponds to an *synchronous* service.
        builder.RegisterService(&(*service_));
        // Finally assemble the server.
        std::unique_ptr<Server> server(builder.BuildAndStart());
        std::cout << "Server listening on " << server_address << std::endl;

        // Wait for the server to shutdown. Note that some other thread must be
        // responsible for shutting down the server for this call to ever return.
        server->Wait();

        KeyValueStoreClient kvclient(grpc::CreateChannel(
            "localhost:50001", grpc::InsecureChannelCredentials()));

        client_ = &kvclient;
        /*
        client.put("One", "1");
        client.put("Two", "2");
        client.put("Three", "3");
        */
    }

    KeyValueStoreServiceImpl *service_;
    KeyValueStoreClient *client_;
};

TEST_F(KvstoreServerTests, GetValues)
{
    EXPECT_EQ(client.get("One"), "1");
    EXPECT_EQ(client.get("Two"), "2");
    EXPECT_EQ(client.get("Three"), "3");
}

TEST_F(KvstoreServerTests, InsertNewValue)
{
    EXPECT_EQ(client.get("Four"), "NULL");
    EXPECT_TRUE(client.put("Four", "4"));
    EXPECT_EQ(client.get("Four"), "4");
}

TEST_F(KvstoreServerTests, RemoveValue)
{

    EXPECT_EQ(client_->get("One"), "1");
    EXPECT_TRUE(client_->remove("One"));
    EXPECT_EQ(client_->get("One"), NULL);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}