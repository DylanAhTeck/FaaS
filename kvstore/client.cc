/*
 *
 * Copyright 2018 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

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

    void put(const std::string &key, const std::string &value)
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

    void remove(const std::string &key)
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

    void get(const std::string &key)
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

private:
    std::unique_ptr<KeyValueStore::Stub> stub_;
};

int main(int argc, char **argv)
{
    KeyValueStoreClient kvclient(grpc::CreateChannel(
        "localhost:50001", grpc::InsecureChannelCredentials()));

    // kvclient.get("hep");
    // kvclient.put("hep", "hi");
    // kvclient.remove("hep");
    // kvclient.get("hep");

    return 0;
}
