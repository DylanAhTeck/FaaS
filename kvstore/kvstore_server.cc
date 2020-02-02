#include <iostream>
#include <memory>
#include <string>
#include <vector>

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

struct kv_pair
{
    const char *key;
    const char *value;
};

// static const kv_pair kvs_map[] = {
//     {"key1", "value1"},
//     {"key2", "value2"},
//     {"key3", "value3"},
//     {"key4", "value4"},
//     {"key5", "value5"},
// };

// const char *get_value_from_map(const char *key)
// {
//     for (size_t i = 0; i < sizeof(kvs_map) / sizeof(kv_pair); ++i)
//     {
//         if (strcmp(key, kvs_map[i].key) == 0)
//         {
//             return kvs_map[i].value;
//         }
//     }
//     return "";
// }

// Logic and data behind the server's behavior.
class KeyValueStoreServiceImpl final : public KeyValueStore::Service
{

    //Status put (PutRequest) returns (PutReply) {}
    //Status get(stream GetRequest) returns(stream GetReply) {}
    //Status remove(RemoveRequest) returns(RemoveReply) {}
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
