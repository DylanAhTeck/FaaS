// Copyright 2020 Dylan Ah Teck

#ifndef SRC_KVSTORE_SERVER_H_
#define SRC_KVSTORE_SERVER_H_

#include <grpcpp/grpcpp.h>

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "database.h"         //NOLINT
#include "kvstore.grpc.pb.h"  //NOLINT

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
// Key-Value Store Service allows for Put, Get and Remove storage functionality
class KeyValueStoreServiceImpl final : public KeyValueStore::Service {
  // Adds a key,value pair to key-value store if key does not yet exist
  // Adds a value to the vector of values if key exists
  // Returns Status::OK if successful, Status::Cancelled if not
  Status put(ServerContext *context, const PutRequest *putrequest,
             PutReply *putreply) override;

  // Writes a stream of key's values or does nothing if key does not exist
  // Always returns Status::OK
  Status get(ServerContext *context,
             ServerReaderWriter<GetReply, GetRequest> *stream) override;

  // Removes a key,value pair from key-value store if key exists, else does
  // nothing Returns Status::OK if request correctly returns value or correctly
  // returns nothing Returns Status::CANCELLED if request fails
  Status remove(ServerContext *context, const RemoveRequest *removerequest,
                RemoveReply *removereply) override;

 private:
  // std::unordered_map<std::string, std::string> umap_;
  Database internal_db_;
};

}  // namespace dylanwarble

#endif  // SRC_KVSTORE_SERVER_H_"
