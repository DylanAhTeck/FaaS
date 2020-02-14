#include <grpcpp/grpcpp.h>

// #ifdef BAZEL_BUILD
// #include "examples/protos/keyvaluestore.grpc.pb.h"
// #else
#include "warble.grpc.pb.h"
//#endif

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReaderWriter;
using grpc::Status;

using warble::FollowReply;
using warble::FollowRequest;
using warble::ProfileReply;
using warble::ProfileRequest;
using warble::ReadReply;
using warble::ReadRequest;
using warble::RegisteruserReply;
using warble::RegisteruserRequest;
using warble::WarbleReply;
using warble::WarbleRequest;

class Warble
{
    //registeruser(username)
    //Registers the given non-blank username
    Status registeruser(ServerContext *context, const RegisteruserRequest *request,
                        RegisteruserReply *reply);

    Status warble(ServerContext *context, const WarbleRequest *request, WarbleReply *reply);

    Status follow(ServerContext *context, const FollowRequest *request, FollowReply *reply);

    Status read(ServerContext *context, const ReadRequest *request, ReadReply *reply);

    Status profile(ServerContext *context, const ProfileRequest *request, ProfileReply *reply);
};