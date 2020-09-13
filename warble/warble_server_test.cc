// Copyright 2020 Dylan Ah Teck

#include <gtest/gtest.h>

#include "warble_server.h"  //NOLINT

// Tests to verify Warble functions
// NOTE: For tests to work, must run kvstore_server
// and restart kvstore_server after each test run
class WarbleServerTests : public ::testing::Test {
 public:
  WarbleServerTests() {}

  ~WarbleServerTests() {}

  void TearDown() {}

  void SetUp() {}
  dylanwarble::WarbleFunctions warble_;
};

// Test adding a new user (success returns true)
TEST_F(WarbleServerTests, TestRegisterNewUserSucceeds) {
  EXPECT_TRUE(warble_.RegisterUser("JaneDoe"));
}

// Test registering an existing user fails (failure returns false)
TEST_F(WarbleServerTests, TestRegisteringExistingUserFails) {
  EXPECT_TRUE(warble_.RegisterUser("JohnSmith"));
  EXPECT_FALSE(warble_.RegisterUser("JohnSmith"));
}

// Test one user following another returns succeeds
TEST_F(WarbleServerTests, TestFollowingAnotherUserSucceeds) {
  EXPECT_TRUE(warble_.RegisterUser("Abby"));
  EXPECT_TRUE(warble_.RegisterUser("Bob"));
  EXPECT_TRUE(warble_.Follow("Abby", "Bob"));
}

// Test one user following themselves returns failure
TEST_F(WarbleServerTests, TestFollowingSelfFails) {
  EXPECT_TRUE(warble_.RegisterUser("Charlie"));
  EXPECT_FALSE(warble_.Follow("Charlie", "Charlie"));
}

// Test profile returns followers and following
TEST_F(WarbleServerTests, TestGettingFollowingAndFollowersFromProfileSucceeds) {
  EXPECT_TRUE(warble_.RegisterUser("Dennis"));
  EXPECT_TRUE(warble_.RegisterUser("Elise"));
  EXPECT_TRUE(warble_.RegisterUser("Finn"));

  EXPECT_TRUE(warble_.Follow("Dennis", "Elise"));
  EXPECT_TRUE(warble_.Follow("Elise", "Finn"));

  std::vector<std::string> *followers = new std::vector<std::string>;
  std::vector<std::string> *following = new std::vector<std::string>;

  warble_.Profile("Elise", followers, following);

  EXPECT_EQ(followers->at(0), "Dennis");
  EXPECT_EQ(following->at(0), "Finn");

  delete followers;
  delete following;
}

// Test profile fails to return followers and following with unregistered user
TEST_F(WarbleServerTests, TestProfileFailsWithInvalidUser) {
  std::vector<std::string> *followers = new std::vector<std::string>;
  std::vector<std::string> *following = new std::vector<std::string>;

  EXPECT_FALSE(warble_.Profile("George", followers, following));

  delete followers;
  delete following;
}

// Test warble fails with unregistered user or with registered user and invalid
// reply id
TEST_F(WarbleServerTests, TestWarbleFailsWithInvalidCommand) {
  WarbleReply *warble_reply = new WarbleReply;

  EXPECT_FALSE(warble_.PostWarble("George", "This is my warble text!",
                                  dylanwarble::kStringNotSet, warble_reply));
  EXPECT_TRUE(warble_.RegisterUser("George"));

  // Valid warble IDs start as from 1
  std::string parent_id = "0";

  EXPECT_FALSE(warble_.PostWarble("George", "This is my second warble text!",
                                  parent_id, warble_reply));

  delete warble_reply;
}

// Test warble succeeds, both with valid user and valid reply
TEST_F(WarbleServerTests, TestWarbleSucceedsWithValidUser) {
  WarbleReply *warble_reply = new WarbleReply;

  EXPECT_TRUE(warble_.RegisterUser("Hazel"));
  EXPECT_TRUE(warble_.PostWarble("Hazel", "This is my first warble!",
                                 dylanwarble::kStringNotSet, warble_reply));

  // The ID of her warble can be retrieved
  std::string parent_id = warble_reply->warble().id();

  EXPECT_TRUE(warble_.PostWarble("Hazel",
                                 "This is my reply to my first warble!",
                                 parent_id, warble_reply));

  EXPECT_EQ(warble_reply->warble().username(), "Hazel");
  EXPECT_EQ(warble_reply->warble().text(),
            "This is my reply to my first warble!");
  EXPECT_EQ(warble_reply->warble().parent_id(), parent_id);

  delete warble_reply;
}

// Test read succeeds in reading an existing warble thread
TEST_F(WarbleServerTests, TestReadSucceedsWithValidID) {
  std::vector<Warble *> *warble_thread = new std::vector<Warble *>;
  WarbleReply *izabel_warble_reply = new WarbleReply;
  WarbleReply *jason_warble_reply = new WarbleReply;

  EXPECT_TRUE(warble_.RegisterUser("Izabel"));
  EXPECT_TRUE(warble_.RegisterUser("Jason"));
  EXPECT_TRUE(warble_.PostWarble("Izabel", "This is my first warble!",
                                 dylanwarble::kStringNotSet,
                                 izabel_warble_reply));

  // Retrieves Izabel's new warble id and parent warble id
  std::string izabel_warble_id = izabel_warble_reply->warble().id();

  EXPECT_TRUE(warble_.PostWarble("Jason", "This my reply to Izabel!",
                                 izabel_warble_id, jason_warble_reply));

  EXPECT_TRUE(warble_.Read(izabel_warble_id, warble_thread));

  // Confirm warble at position 0 is Iza's, and warble at position 2 is Jason's
  EXPECT_EQ(warble_thread->at(0)->text(), "This is my first warble!");
  EXPECT_EQ(warble_thread->at(0)->username(), "Izabel");
  EXPECT_EQ(warble_thread->at(1)->text(), "This my reply to Izabel!");
  EXPECT_EQ(warble_thread->at(1)->username(), "Jason");
  EXPECT_EQ(warble_thread->at(1)->parent_id(), izabel_warble_id);

  delete warble_thread;
  delete izabel_warble_reply;
  delete jason_warble_reply;
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
