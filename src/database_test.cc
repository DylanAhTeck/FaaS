#include "database.h"
#include <gtest/gtest.h>

class DatabaseTests : public ::testing::Test
{

public:
    DatabaseTests()
    {
    }

    ~DatabaseTests() override
    {
    }

    void TearDown()
    {
        delete db;
    }

    void SetUp() override
    {
        db = new Database();
        db->put("OneTwoThree", "One");
        db->put("OneTwoThree", "Two");
        db->put("OneTwoThree", "Three");
    }

    Database *db;
};

//Test that getting value works
TEST_F(DatabaseTests, GetValues)
{
    std::vector<std::string> values = db->get("OneTwoThree");

    EXPECT_EQ(values[0], "One");
    EXPECT_EQ(values[1], "Two");
    EXPECT_EQ(values[2], "Three");
}

//More tests to come

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}