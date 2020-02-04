//#include "caching_interceptor.h"

// #ifdef BAZEL_BUILD
// #include "examples/protos/keyvaluestore.grpc.pb.h"
// #else
// #include "../kvstore/keyvaluestore.grpc.pb.h"
// #endif

//#include "../kvstore/kvstore.grpc.pb.h"
#include "kvstore.grpc.pb.h"
#include "client.h"

void KeyValueStoreClient::put(const std::string &key, const std::string &value)
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
    {
        std::cout << "PUT" << std::endl;
        //return reply.message();
    }
    else
    {
        //   std::cout << status.error_code() << ": " << status.error_message()
        //             << std::endl;
        //   return "RPC failed";
    }
}

void KeyValueStoreClient::remove(const std::string &key)
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
    {
        std::cout << "REMOVED" << std::endl;
        //return reply.message();
    }
}

void KeyValueStoreClient::get(const std::string &key)
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
    std::cout << key << " : " << reply.value() << "\n";

    stream->WritesDone();
    Status status = stream->Finish();
    if (!status.ok())
    {
        std::cout << status.error_code() << ": " << status.error_message()
                  << std::endl;
        std::cout << "RPC failed";
    }
}

int main(int argc, char **argv)
{
    KeyValueStoreClient kvclient(grpc::CreateChannel(
        "localhost:50001", grpc::InsecureChannelCredentials()));

    kvclient.get("hep");
    kvclient.put("hi", "hi");
    kvclient.get("hi");
    kvclient.remove("hep");
    kvclient.get("hep");

    return 0;
}
