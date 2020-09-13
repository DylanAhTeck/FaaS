// Copyright 2020 Dylan Ah Teck
#ifndef SRC_FUNC_SERVER_H_
#define SRC_FUNC_SERVER_H_

#include <glog/logging.h>
#include <grpcpp/grpcpp.h>

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "func.grpc.pb.h"   //NOLINT
#include "warble_server.h"  //NOLINT

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReaderWriter;
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

namespace dylanwarble {

// Internal data structure to maintain hooked and unhooked functions
struct Event {
  Event(int event_id, std::string function_name, bool yes)
      : event_type(event_id), event_function(function_name), hooked(yes) {}

  int event_type;
  std::string event_function;
  bool hooked;
};

class FuncServiceImpl final : public FuncService::Service {
  // Allows a function to be added to Func service
  // Return Status::OK if successful, Status::CANCELLED if not
  Status hook(ServerContext *context, const HookRequest *hookrequest,
              HookReply *hookreply) override;

  // Removes a function from Func service
  // Return Status::OK if successful, Status::CANCELLED if not
  Status unhook(ServerContext *context, const UnhookRequest *unhookrequest,
                UnhookReply *unhookreply) override;

  // Receives events of a particular type with a payload, processes request
  // Return Status::OK if successful, Status::CANCELLED if not
  Status event(ServerContext *context, const EventRequest *eventrequest,
               EventReply *eventreply) override;

 public:
  // Sets up initial functions for Warble service
  void HookInitialWarbleFunctions();

 private:
  std::vector<Event> event_table_;
};

}  // namespace dylanwarble

#endif  // SRC_FUNC_SERVER_H_
