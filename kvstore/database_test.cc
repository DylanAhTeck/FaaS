// Copyright 2020 Dylan Ah Teck

#include <gtest/gtest.h>

#include "database.h"  //NOLINT

// Tests to verify GET, PUT and REMOVE functionality of internal Database class
class DatabaseTests : public ::testing::Test {
 public:
  DatabaseTests() {}

  ~DatabaseTests() {}

  void TearDown() {}

  // Inputs 3 key-value pairs before every test
  void SetUp() {
    db.Put("OneTwoThree", "One");
    db.Put("OneTwoThree", "Two");
    db.Put("OneTwoThree", "Three");
  }
  dylanwarble::Database db;
};

// Test that getting value works
TEST_F(DatabaseTests, TestGetWithInitialValues) {
  std::vector<std::string> values = db.Get("OneTwoThree");
  EXPECT_EQ(values[0], "One");
  EXPECT_EQ(values[1], "Two");
  EXPECT_EQ(values[2], "Three");
}

// Test that removing initial values works
TEST_F(DatabaseTests, TestRemoveWithInitialValues) {
  db.Remove("OneTwoThree");
  EXPECT_TRUE(db.Get("OneTwoThree").empty());
}

// Test put into same key with initial values
TEST_F(DatabaseTests, TestPutSameKey) {
  db.Put("OneTwoThree", "Four");

  std::vector<std::string> values = db.Get("OneTwoThree");

  EXPECT_EQ(values[0], "One");
  EXPECT_EQ(values[1], "Two");
  EXPECT_EQ(values[2], "Three");
  EXPECT_EQ(values[3], "Four");
}

// Test put of new key-value pair
TEST_F(DatabaseTests, TestPutNewKeyValuePair) {
  db.Put("FourFive", "Four");
  db.Put("FourFive", "Five");

  std::vector<std::string> values = db.Get("FourFive");

  EXPECT_EQ(values[0], "Four");
  EXPECT_EQ(values[1], "Five");
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
