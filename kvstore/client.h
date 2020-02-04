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
#include "kvstore.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using kvstore::GetReply;
using kvstore::GetRequest;
using kvstore::KeyValueStore;
using kvstore::PutReply;
using kvstore::PutRequest;
using kvstore::RemoveReply;
using kvstore::RemoveRequest;

class KeyValueStoreClient
{
public:
    KeyValueStoreClient(std::shared_ptr<Channel> channel)
        : stub_(KeyValueStore::NewStub(channel)) {}

    void put(const std::string &key, const std::string &value);

    void remove(const std::string &key);
    void get(const std::string &key);

private:
    std::unique_ptr<KeyValueStore::Stub> stub_;
};