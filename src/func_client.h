#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <grpcpp/grpcpp.h>
#include "func.grpc.pb.h"
#include "warble.grpc.pb.h"
#include "enums.h"

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
using warble::Warble;
using warble::WarbleReply;
using warble::WarbleRequest;

namespace dylanwarble
{

// Internal datastructure to package data from clclient to func_client
struct Payload
{
  int event_type;
  std::string event_function;

  std::string username;
  std::string text;
  std::string parent_id;
  std::string id;
  std::string to_follow;
};
// Internal datastructure to package response data
struct CommandResponse
{
  CommandResponse() : success(false){};

  int event_type;
  bool success;

  //Each element in vector is a warble in thread specified
  std::vector<std::string> warble_threads;

  //Each element is the name of one of user's followers
  std::vector<std::string> followers;

  //Each element is the name of one of user's following
  std::vector<std::string> following;
};

class FuncClient
{
public:
  // Creates a new client
  FuncClient(std::shared_ptr<Channel> channel)
      : stub_(FuncService::NewStub(channel)) {}

  // Hooks an event with int event_type
  void Hook(const int event_type, const std::string &event_function);

  // Unooks an event with int event_type
  void Unhook(const int event_type);

  // Requests an event with parameters stored in Payload by clclient
  void Event(const int event_type, const Payload &p, CommandResponse &r);

private:
  std::unique_ptr<FuncService::Stub> stub_;
};

} // namespace dylanwarble