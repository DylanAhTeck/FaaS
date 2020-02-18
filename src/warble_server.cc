#include <grpcpp/grpcpp.h>
#include "warble_server.h"

#include "warble.grpc.pb.h"

Status Warble::registeruser(ServerContext *context, const RegisteruserRequest *request,
                            RegisteruserReply *reply)
{
}

Status Warble::warble(ServerContext *context, const WarbleRequest *request, WarbleReply *reply)
{
}

Status Warble::follow(ServerContext *context, const FollowRequest *request, FollowReply *reply)
{
}

Status Warble::read(ServerContext *context, const ReadRequest *request, ReadReply *reply)
{
}

Status Warble::profile(ServerContext *context, const ProfileRequest *request, ProfileReply *reply)
{
}

int main()
{
    return 1;
}