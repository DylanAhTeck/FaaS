#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <grpcpp/grpcpp.h>
#include "kvstore_server.h"

// #ifdef BAZEL_BUILD
// #include "examples/protos/keyvaluestore.grpc.pb.h"
// #else
#include "kvstore.grpc.pb.h"
//#endif

namespace dylanwarble {

Status KeyValueStoreServiceImpl::put(ServerContext *context,
                                     const PutRequest *putrequest,
                                     PutReply *putreply) {
  std::string key = putrequest->key();
  std::string value = putrequest->value();
  bool success = internal_db_.Put(key, value);

  if (success)
    return Status::OK;
  else
    return Status::CANCELLED;
}

Status KeyValueStoreServiceImpl::get(
    ServerContext *context, ServerReaderWriter<GetReply, GetRequest> *stream) {
  // Major Questions:
  GetRequest request;

  while (stream->Read(&request)) {
    GetReply reply;
    std::vector<std::string> values = internal_db_.Get(request.key());
    for (int i = 0; i < values.size(); i++) {
      std::string s = values[i];
      reply.set_value(s);
      stream->Write(reply);
    }
  }
  return Status::OK;
}

Status KeyValueStoreServiceImpl::remove(ServerContext *context,
                                        const RemoveRequest *removerequest,
                                        RemoveReply *removereply) {
  bool success = internal_db_.Remove(removerequest->key());

  if (success) {
    VLOG(google::INFO) << "Key-value store remove request was successfull.";
    return Status::OK;
  }

  else {
    VLOG(google::ERROR) << "Key-value store remove request failed.";
    return Status::CANCELLED;
  }
}

}  // namespace dylanwarble

void RunServer() {
  std::string server_address("0.0.0.0:50001");
  dylanwarble::KeyValueStoreServiceImpl service;

  ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case, it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Key-value store server listening on " << server_address
            << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

int main(int argc, char **argv) {
  google::InitGoogleLogging(argv[0]);
  FLAGS_logtostderr = 1;
  RunServer();
  return 0;
}
