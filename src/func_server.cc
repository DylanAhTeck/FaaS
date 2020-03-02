#include "func_server.h"
#include <grpcpp/grpcpp.h>

#include "func.grpc.pb.h"

namespace dylanwarble {
Status FuncServiceImpl::hook(ServerContext *context,
                             const HookRequest *hookrequest,
                             HookReply *hookreply) {
  return Status::OK;
}

Status FuncServiceImpl::unhook(ServerContext *context,
                               const UnhookRequest *unhookrequest,
                               UnhookReply *unhookreply) {
  return Status::OK;
}

Status FuncServiceImpl::event(ServerContext *context,
                              const EventRequest *eventrequest,
                              EventReply *eventreply) {
  const google::protobuf::Any payload = eventrequest->payload();

  RegisteruserRequest request;
  payload.UnpackTo(&request);
  std::string s = request.username();
  std::cerr << s << std::endl;

  return Status::OK;
}

void RunServer() {
  std::string server_address("0.0.0.0:50000");
  FuncServiceImpl service;

  ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case, it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

}  // namespace dylanwarble

int main(int argc, char **argv) {
  dylanwarble::RunServer();
  return 0;
}
