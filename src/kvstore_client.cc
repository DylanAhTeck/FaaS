//#include "caching_interceptor.h"

// #ifdef BAZEL_BUILD
// #include "examples/protos/keyvaluestore.grpc.pb.h"
// #else
// #include "../kvstore/keyvaluestore.grpc.pb.h"
// #endif

//#include "../kvstore/kvstore.grpc.pb.h"
#include "kvstore.grpc.pb.h"
#include "kvstore_client.h"

bool KeyValueStoreClient::put(const std::string &key, const std::string &value)
{
    // Data we are sending to the server.
    PutRequest request;
    request.set_value(value);
    request.set_key(key);

    // Container for the data we expect from the server.
    PutReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->put(&context, request, &reply);

    // Act upon its status.
    if (status.ok())
        return true;

    return false;
}

bool KeyValueStoreClient::remove(const std::string &key)
{
    // Data we are sending to the server.
    RemoveRequest request;
    request.set_key(key);

    // Container for the data we expect from the server.
    RemoveReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->remove(&context, request, &reply);

    // Act upon its status.
    if (status.ok())
        return true;

    return false;
}

std::string KeyValueStoreClient::get(const std::string &key)
{
    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;
    auto stream = stub_->get(&context);

    GetRequest request;
    request.set_key(key);
    stream->Write(request);

    // Get the value for the sent key
    GetReply reply;
    stream->Read(&reply);
    //std::cout << key << " : " << reply.value() << "\n";

    stream->WritesDone();
    Status status = stream->Finish();
    if (status.ok())
        return reply.value();

    return NULL;
}

int main(int argc, char **argv)
{
    KeyValueStoreClient kvclient(grpc::CreateChannel(
        "localhost:50001", grpc::InsecureChannelCredentials()));

    //Random order of functions for quick initial test
    kvclient.get("hep");
    kvclient.put("hi", "hi");
    kvclient.put("hi", "hello");
    kvclient.get("hi");
    kvclient.remove("hi");
    kvclient.get("hi");

    return 0;
}
