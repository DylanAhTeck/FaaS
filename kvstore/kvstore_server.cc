#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include <grpcpp/grpcpp.h>

// #ifdef BAZEL_BUILD
// #include "examples/protos/keyvaluestore.grpc.pb.h"
// #else
#include "kvstore.grpc.pb.h"
//#endif

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReaderWriter;
using grpc::Status;

using kvstore::GetReply;
using kvstore::GetRequest;
using kvstore::KeyValueStore;
using kvstore::PutReply;
using kvstore::PutRequest;
using kvstore::RemoveReply;
using kvstore::RemoveRequest;

std::unordered_map<std::string, std::string> umap;

// Logic and data behind the server's behavior.
class KeyValueStoreServiceImpl final : public KeyValueStore::Service
{

    Status put(ServerContext *context, const PutRequest *putrequest,
               PutReply *putreply) override
    {
        umap.insert(std::make_pair(putrequest->key(), putrequest->value()));
        if (umap.find(putrequest->key()) == umap.end())
            return Status::CANCELLED;
        return Status::OK;
    }

    Status get(ServerContext *context, ServerReaderWriter<GetReply, GetRequest> *stream)
    {
        GetRequest request;
        while (stream->Read(&request))
        {
            GetReply reply;
            reply.set_value(umap[request.key()]);
            stream->Write(reply);
        }
        return Status::OK;
    }

    Status remove(ServerContext *context, const RemoveRequest *removerequest,
                  RemoveReply *removereply) override
    {
        umap.erase(removerequest->key());
        if (umap.find(removerequest->key()) == umap.end())
            return Status::OK;
        return Status::CANCELLED;
    }
};

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
