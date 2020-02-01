#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using func::Greeter;
using func::HelloRequest;
using func::HelloReply;