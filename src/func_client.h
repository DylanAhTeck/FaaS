// Copyright 2020 Dylan Ah Teck

#ifndef SRC_FUNC_CLIENT_H_
#define SRC_FUNC_CLIENT_H_

#include <glog/logging.h>
#include <grpcpp/grpcpp.h>

#include <memory>
#include <string>
#include <vector>

#include "enums.h"           // NOLINT
#include "func.grpc.pb.h"    // NOLINT
#include "warble.grpc.pb.h"  // NOLINT

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using func::EventReply;
using func::EventRequest;
using func::FuncService;
using func::HookReply;
using func::HookRequest;
using func::UnhookReply;
using func::UnhookRequest;

using warble::FollowReply;
using warble::FollowRequest;
using warble::ProfileReply;
using warble::ProfileRequest;
using warble::ReadReply;
using warble::ReadRequest;
using warble::RegisteruserReply;
using warble::RegisteruserRequest;
using warble::Timestamp;
using warble::Warble;
using warble::WarbleReply;
using warble::WarbleRequest;

namespace dylanwarble {
// NOLINT Internal datastructure to package data from clclient to func_client
struct Payload {
  int event_type;
  std::string event_function;

  std::string username;
  std::string text;
  std::string parent_id;
  std::string id;
  std::string to_follow;
};
// Internal datastructure to package response data
struct CommandResponse {
  CommandResponse() : success(false) {}

  int event_type;
  bool success;

  int timestamp_seconds;
  int timestamp_u_seconds;

  std::string warbleID;
  std::string username;
  std::string reply_id;
  std::string warble_text;
  std::string parent_id;

  // Each element in vector is a warble in thread specified
  std::vector<Warble> warble_threads;

  // Each element is the name of one of user's followers
  std::vector<std::string> followers;

  // Each element is the name of one of user's following
  std::vector<std::string> following;
};
// Class to provide func interface/decoupling
// Used for clclient to call
class FuncClient {
 public:
  // Creates a new client
  explicit FuncClient(std::shared_ptr<Channel> channel)
      : stub_(FuncService::NewStub(channel)) {}

  // Hooks an event with int event_type
  bool Hook(const int event_type, const std::string event_function);

  // Unooks an event with int event_type
  bool Unhook(const int event_type);

  // Requests an event with parameters stored in Payload by clclient
  void Event(const int event_type, const Payload *p, CommandResponse *r);

 private:
  std::unique_ptr<FuncService::Stub> stub_;
};

}  // namespace dylanwarble

#endif  // SRC_FUNC_CLIENT_H_
