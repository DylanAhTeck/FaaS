// Copyright 2020 Dylan Ah Teck

#include "kvstore_server.h"  //NOLINT
namespace dylanwarble {

// Adds a key,value pair to key-value store if key does not yet exist
// Adds a value to the vector of values if key exists
// Returns Status::OK if successful, Status::Cancelled if not
Status KeyValueStoreServiceImpl::put(ServerContext *context,
                                     const PutRequest *putrequest,
                                     PutReply *putreply) {
  std::string key = putrequest->key();
  std::string value = putrequest->value();

  // Calls internal database's PUT function and stores success flag
  bool success = internal_db_.Put(key, value);

  // Note - can't print value in VLOG since warble string values are serialized
  if (success) {
    VLOG(google::INFO) << "SUCCESS: Key-value store PUT (key = '" << key
                       << "') was successful.";
    return Status::OK;
  } else {
    VLOG(google::ERROR) << "FAILURE: Key-value store PUT (key = '" << key
                        << "') failed.";
    return Status::CANCELLED;
  }
}

// Writes a stream of key's values back to kv_client
// Always returns Status::OK
Status KeyValueStoreServiceImpl::get(
    ServerContext *context, ServerReaderWriter<GetReply, GetRequest> *stream) {
  GetRequest getrequest;

  std::string key;
  // Reads in requests via stream
  while (stream->Read(&getrequest)) {
    GetReply reply;
    key = getrequest.key();

    // Calls internal database's GET function and stores values in vector
    std::vector<std::string> values = internal_db_.Get(key);
    for (int i = 0; i < values.size(); i++) {
      std::string s = values[i];
      // Stores values in reply
      reply.set_value(s);
      // Writes reply back via stream
      stream->Write(reply);
    }
  }

  VLOG(google::INFO) << "SUCCESS: Key-value store GET (key = '" << key
                     << "') was successful.";
  return Status::OK;
}

// Removes a key,value pair from key-value store if key exists, else does
// nothing Returns Status::OK if request correctly returns value or correctly
// returns nothing Returns Status::CANCELLED if request fails
Status KeyValueStoreServiceImpl::remove(ServerContext *context,
                                        const RemoveRequest *removerequest,
                                        RemoveReply *removereply) {
  // Calls internal database's REMOVE function and stores success flag
  bool success = internal_db_.Remove(removerequest->key());

  std::string key = removerequest->key();

  if (success) {
    VLOG(google::INFO) << "SUCCESS: Key-value store REMOVE (key = '" << key
                       << "') was successful.";
    return Status::OK;
  } else {
    VLOG(google::ERROR) << "FAILURE: Key-value store REMOVE (key = '" << key
                        << "') failed.";
    return Status::CANCELLED;
  }
}

}  // namespace dylanwarble

void RunServer() {
  // Declare port number
  std::string server_address("0.0.0.0:50001");
  dylanwarble::KeyValueStoreServiceImpl service;

  ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients.
  builder.RegisterService(&service);
  // Finally assemble the server.
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Key-value store server listening on " << server_address
            << std::endl;

  // Wait for the server to shutdown.
  server->Wait();
}

int main(int argc, char **argv) {
  // Set google logging
  google::InitGoogleLogging(argv[0]);

  // Run server
  RunServer();
  return 0;
}
