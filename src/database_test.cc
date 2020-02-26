#include "database.h"
#include <gtest/gtest.h>

//TESTS ON HOLD FOR NOW - does not compile 

//Tests to verify GET, PUT and REMOVE functionality of Database class
class DatabaseTests : public ::testing::Test {
 public:
  DatabaseTests() {}

  ~DatabaseTests() {}

  void TearDown() { delete db; }

  //Inputs 3 key-value pairs before every test
  void SetUp() {
    db = new Database();
    db->Put("OneTwoThree", "One");
    db->Put("OneTwoThree", "Two");
    db->Put("OneTwoThree", "Three");
  }

  Database *db;
};

// Test that getting value works
TEST_F(DatabaseTests, getSetUpValuesToValidateGet) {
  std::vector<std::string> values = db->Get("OneTwoThree");

  EXPECT_EQ(values[0], "One");
  EXPECT_EQ(values[1], "Two");
  EXPECT_EQ(values[2], "Three");
}


// More tests to come

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}