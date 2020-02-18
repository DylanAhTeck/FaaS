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
    //Initializes client with function stubs
    KeyValueStoreClient(std::shared_ptr<Channel> channel)
        : stub_(KeyValueStore::NewStub(channel)) {}
    //Sends PUT request with key,value variables to call kvstore_server API
    //Returns true if remove was successful
    bool put(const std::string &key, const std::string &value);

    //Sends REMOVE request with key variable to call kvstore_server API
    //Returns true if remove was succesful
    bool remove(const std::string &key);

    //Returns a previously stored value or values under that key or NULL if the key
    //is not present in the store
    std::string get(const std::string &key);

private:
    std::unique_ptr<KeyValueStore::Stub> stub_;
};