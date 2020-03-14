// Copyright 2020 Dylan Ah Teck

#include <gtest/gtest.h>

#include "func_client.h"  //NOLINT

// Tests to verify func_client functions
// Note: must run AND RESTART ./func_server and ./kvstore_server
// for every run-through
class FuncClientTests : public ::testing::Test {
 public:
  FuncClientTests() {}
  ~FuncClientTests() {}
  void TearDown() {
    delete response_;
    delete payload_;
  }
  void SetUp() {
    this->response_ = new dylanwarble::CommandResponse;
    this->payload_ = new dylanwarble::Payload;

    dylanwarble::FuncClient funcclient(grpc::CreateChannel(
        "localhost:50000", grpc::InsecureChannelCredentials()));

    // Registers a common user to use for tests
    // VLOG may note that the tests attempt to recreate an existing user

    this->payload_->username = "AliceSmith";
    funcclient.Event(dylanwarble::kRegisterUserID, this->payload_,
                     this->response_);

    // Resets username field to empty
    this->payload_->username = "";
  }

  dylanwarble::CommandResponse *response_;
  dylanwarble::Payload *payload_;
};

// Verify hooking new valid function succeeds
TEST_F(FuncClientTests, HookingNewFunctionSucceeds) {
  dylanwarble::FuncClient funcclient(grpc::CreateChannel(
      "localhost:50000", grpc::InsecureChannelCredentials()));

  // Note: func_server sets up 5 initial functions when it is first set up
  // Therefore the next available event_type is 5
  int event_type = 5;
  const std::string event_function = "TestFunction";

  EXPECT_TRUE(funcclient.Hook(event_type, event_function));
}

// Verify hooking new function with event_type already used fails
TEST_F(FuncClientTests, HookingWithExistingEventTypeFails) {
  dylanwarble::FuncClient funcclient(grpc::CreateChannel(
      "localhost:50000", grpc::InsecureChannelCredentials()));

  // Note: func_server sets up 5 initial functions when it is first set up
  // Therefore event_type 0 to 4 are already in use
  int event_type = 0;
  const std::string event_function = "TestFunction";

  EXPECT_FALSE(funcclient.Hook(event_type, event_function));
}

// Verify unhooking new valid function succeeds (i.e existing event_type)
TEST_F(FuncClientTests, UnhookingExistingFunctionSucceeds) {
  dylanwarble::FuncClient funcclient(grpc::CreateChannel(
      "localhost:50000", grpc::InsecureChannelCredentials()));

  // Note: func_server sets up 5 initial functions when it is first set up
  // Therefore event_type 0 can be unhooked
  int event_type = 0;

  EXPECT_TRUE(funcclient.Unhook(event_type));
}

// Verify unhooking new invalid function fails (i.e invalid event_type)
TEST_F(FuncClientTests, UnhookingInvalidFunctionFails) {
  dylanwarble::FuncClient funcclient(grpc::CreateChannel(
      "localhost:50000", grpc::InsecureChannelCredentials()));

  // Note: func_server sets up 5 initial functions when it is first set up
  int event_type = 10;

  EXPECT_FALSE(funcclient.Unhook(event_type));
}

// Test Register Event success
TEST_F(FuncClientTests, TestRegisterEventSucceeds) {
  dylanwarble::FuncClient funcclient(grpc::CreateChannel(
      "localhost:50000", grpc::InsecureChannelCredentials()));

  this->payload_->username = "JohnSmith";
  funcclient.Event(dylanwarble::kRegisterUserID, this->payload_,
                   this->response_);
  ASSERT_TRUE(this->response_->success);
}

// Test WarbleEvent success
TEST_F(FuncClientTests, TestWarbleEventSucceeds) {
  dylanwarble::FuncClient funcclient(grpc::CreateChannel(
      "localhost:50000", grpc::InsecureChannelCredentials()));

  this->payload_->username = "AliceSmith";
  this->payload_->text = "This is my warble!";
  this->payload_->parent_id = dylanwarble::kStringNotSet;

  funcclient.Event(dylanwarble::kWarbleID, this->payload_, this->response_);

  ASSERT_EQ(this->response_->username, "AliceSmith");
  ASSERT_EQ(this->response_->warble_text, "This is my warble!");
  ASSERT_TRUE(this->response_->success);
}

// Test ReadEvent success, given TestWarbleEventSucceeds above was successful
TEST_F(FuncClientTests, TestReadEventSucceeds) {
  dylanwarble::FuncClient funcclient(grpc::CreateChannel(
      "localhost:50000", grpc::InsecureChannelCredentials()));

  // The warble posted above is the first warble, hence it has ID 1
  this->payload_->id = "1";

  funcclient.Event(dylanwarble::kReadID, this->payload_, this->response_);

  std::string username = this->response_->warble_threads.at(0).username();
  std::string text = this->response_->warble_threads.at(0).text();
  EXPECT_EQ(username, "AliceSmith");
  EXPECT_EQ(text, "This is my warble!");
  EXPECT_TRUE(this->response_->success);
}

// Test Follow success, given TestRegisterUserEvent above was successful
TEST_F(FuncClientTests, TestFollowEventSucceeds) {
  dylanwarble::FuncClient funcclient(grpc::CreateChannel(
      "localhost:50000", grpc::InsecureChannelCredentials()));

  // AliceSmith follow JohnSmith
  this->payload_->username = "AliceSmith";
  this->payload_->to_follow = "JohnSmith";

  funcclient.Event(dylanwarble::kFollowUserID, this->payload_, this->response_);

  ASSERT_TRUE(this->response_->success);
}

// Test profile success, given TestFollowEventSucceeds above was successful
TEST_F(FuncClientTests, TestProfileSuccess) {
  dylanwarble::FuncClient funcclient(grpc::CreateChannel(
      "localhost:50000", grpc::InsecureChannelCredentials()));

  // AliceSmith follow JohnSmith
  this->payload_->username = "AliceSmith";

  funcclient.Event(dylanwarble::kProfileID, this->payload_, this->response_);

  EXPECT_EQ(this->response_->following.at(0), "JohnSmith");
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
