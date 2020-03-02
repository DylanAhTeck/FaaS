#include <unordered_map>
#include "warble.grpc.pb.h"
#include <grpcpp/grpcpp.h>

// #ifdef BAZEL_BUILD
// #include "examples/protos/keyvaluestore.grpc.pb.h"
// #else
#include "func.grpc.pb.h"
//#endif

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

namespace dylanwarble
{
class FuncServiceImpl final : public FuncService::Service
{
  // Allows a service to specify a function
  // for processing of certain message types
  Status hook(ServerContext *context, const HookRequest *hookrequest,
              HookReply *hookreply) override;

  // Removes/unhooks a function from executing on a particular message type
  Status unhook(ServerContext *context, const UnhookRequest *unhookrequest,
                UnhookReply *unhookreply) override;

  // Receives events of a particular type with a payload
  Status event(ServerContext *context, const EventRequest *eventrequest,
               EventReply *eventreply) override;
};
} // namespace dylanwarble