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

class KeyValueStoreServiceImpl final : public KeyValueStore::Service
{

    Status put(ServerContext *context, const PutRequest *putrequest,
               PutReply *putreply) override;

    Status get(ServerContext *context, ServerReaderWriter<GetReply, GetRequest> *stream) override;

    Status remove(ServerContext *context, const RemoveRequest *removerequest,
                  RemoveReply *removereply) override;

private:
    std::unordered_map<std::string, std::string> umap_;
};