#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <grpcpp/grpcpp.h>

//#include "caching_interceptor.h"

// #ifdef BAZEL_BUILD
// #include "examples/protos/keyvaluestore.grpc.pb.h"
// #else
// #include "../kvstore/keyvaluestore.grpc.pb.h"
// #endif

//#include "../kvstore/kvstore.grpc.pb.h"
#include "func.grpc.pb.h"

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

class FuncClient
{
public:
    //Initializes client with function stubs
    FuncClient(std::shared_ptr<Channel> channel)
        : stub_(FuncService::NewStub(channel)) {}

    //Sends PUT request with key,value variables to call kvstore_server API
    //Receives Status::OK from return status if successful
    void hook(const int event_type, const std::string &event_function);

    //Sends REMOVE request with key variable to call kvstore_server API
    //Receives Status::OK from return status if successful
    void unhook(const int event_type);

    //Sends REMOVE request with key variable to call kvstore_server API
    //Receives value via status reply if get is succesful
    void event(const int event_type, const Payload &p);

private:
    std::unique_ptr<FuncService::Stub> stub_;
};

struct Payload
{
    std::string username;
    std::string text;
    int warbleid;
    std::string usertofollow;
};