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

Status KeyValueStoreServiceImpl::put(ServerContext *context,
                                     const PutRequest *putrequest,
                                     PutReply *putreply)
{
  umap_.insert(std::make_pair(putrequest->key(), putrequest->value()));

  if (umap_.find(putrequest->key()) == umap_.end())
    return Status::CANCELLED;
  return Status::OK;
}

Status KeyValueStoreServiceImpl::get(
    ServerContext *context, ServerReaderWriter<GetReply, GetRequest> *stream)
{

  //Major Questions: 
  GetRequest request;
  while (stream->Read(&request))
  {
    GetReply reply;
    reply.set_value(umap_[request.key()]);
    std::cerr << "Value:" << umap_[request.key()] << std::endl;
    stream->Write(reply);
  }
  return Status::OK;
}

Status KeyValueStoreServiceImpl::remove(ServerContext *context,
                                        const RemoveRequest *removerequest,
                                        RemoveReply *removereply)
{
  umap_.erase(removerequest->key());
  if (umap_.find(removerequest->key()) == umap_.end())
    return Status::OK;
  return Status::CANCELLED;
}

void RunServer()
{
  std::string server_address("0.0.0.0:50001");
  KeyValueStoreServiceImpl service;

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

int main(int argc, char **argv)
{
  RunServer();
  return 0;
}
