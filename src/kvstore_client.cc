// Copyright 2020 Dylan Ah Teck

#include "kvstore_client.h"   //NOLINT
#include "enums.h"            //NOLINT
#include "kvstore.grpc.pb.h"  //NOLINT
namespace dylanwarble {

// Sends PUT request with key,value variables to call Key-Value Store server
// Returns true if remove was successful
bool KeyValueStoreClient::Put(const std::string &key,
                              const std::string &value) {
  // Data we are sending to the server.
  PutRequest request;
  request.set_value(value);
  request.set_key(key);

  // Container for the data we expect from the server.
  PutReply reply;

  // Context for the client.
  ClientContext context;

  // Use RPC
  Status status = stub_->put(&context, request, &reply);

  // Act upon its status.
  if (status.ok()) {
    return true;
  }

  return false;
}

// Sends REMOVE request with key variable to call Key-Value Store server
// Returns true if remove was successful
bool KeyValueStoreClient::Remove(const std::string &key) {
  // Data we are sending to the server.
  RemoveRequest request;
  request.set_key(key);

  // Container for the data we expect from the server.
  RemoveReply reply;

  // Context for the client.
  ClientContext context;

  // Use RPC
  Status status = stub_->remove(&context, request, &reply);

  // Act upon its status.
  if (status.ok()) {
    return true;
  }

  return false;
}

// Returns a previously stored value or values under that key or NULL if the
// key is not present in the store
std::vector<std::string> KeyValueStoreClient::Get(const std::string &key) {
  ClientContext context;

  // Shared ReaderWriter for bi-directional stream
  std::shared_ptr<ClientReaderWriter<GetRequest, GetReply>> stream(
      stub_->get(&context));

  GetRequest request;
  request.set_key(key);
  stream->Write(request);
  stream->WritesDone();

  // Reads in replies for Key-Value Store server and stores in response
  GetReply reply;
  std::vector<std::string> response;
  while (stream->Read(&reply)) {
    std::string value(reply.value());
    response.push_back(value);
  }
  Status status = stream->Finish();

  if (status.ok()) {
    return response;
  }

  // Returns empty string if not successful
  return std::vector<std::string>();
}

}  // namespace dylanwarble
