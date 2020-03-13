#include <unordered_map>

#include <grpcpp/grpcpp.h>

// #ifdef BAZEL_BUILD
// #include "examples/protos/keyvaluestore.grpc.pb.h"
// #else
#include "database.h"
#include "kvstore.grpc.pb.h"
//#endif

using grpc::ClientReaderWriter;
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

namespace dylanwarble {
class KeyValueStoreServiceImpl final : public KeyValueStore::Service {
  // Puts a key,value pair in kvstore internal hashmap
  // Returns Status::OK if succesfull Status::Cancelled if not
  Status put(ServerContext *context, const PutRequest *putrequest,
             PutReply *putreply) override;

  // Returns a previously store value from kvstore internal hashmap
  // based on whether Request key; returns nothing if key is not present

  Status get(ServerContext *context,
             ServerReaderWriter<GetReply, GetRequest> *stream) override;

  // Removes a key,value pair based on Request key
  // Returns Status::OK if succesfull Status::Cancelled if not
  Status remove(ServerContext *context, const RemoveRequest *removerequest,
                RemoveReply *removereply) override;

 private:
  // std::unordered_map<std::string, std::string> umap_;
  Database internal_db_;
};

}  // namespace dylanwarble
