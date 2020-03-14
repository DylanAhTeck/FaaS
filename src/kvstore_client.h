// Copyright 2020 Dylan Ah Teck

#ifndef SRC_KVSTORE_CLIENT_H_
#define SRC_KVSTORE_CLIENT_H_

#include <grpcpp/grpcpp.h>

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "kvstore.grpc.pb.h" //NOLINT

namespace dylanwarble
{

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReaderWriter;
using grpc::Status;

using kvstore::GetReply;
using kvstore::GetRequest;
using kvstore::KeyValueStore;
using kvstore::PutReply;
using kvstore::PutRequest;
using kvstore::RemoveReply;
using kvstore::RemoveRequest;

// Provides an interface to call key-value store service
class KeyValueStoreClient
{
public:
  // Initializes client with function stubs
  explicit KeyValueStoreClient(std::shared_ptr<Channel> channel)
      : stub_(KeyValueStore::NewStub(channel)) {}

  // Sends PUT request with key,value variables to call Key-Value Store server
  // Returns true if put was successful
  bool Put(const std::string &key, const std::string &value);

  // Sends REMOVE request with key variable to call Key-Value Store server
  // Returns true if remove was successful
  bool Remove(const std::string &key);

  // Returns a previously stored value or values under that key or NULL if the
  // key is not present in the store
  std::vector<std::string> Get(const std::string &key);

private:
  // Service stub
  std::unique_ptr<KeyValueStore::Stub> stub_;
};

} // namespace dylanwarble

#endif // SRC_KVSTORE_CLIENT_H_
