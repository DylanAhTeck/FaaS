#include <glog/logging.h>
#include <grpcpp/grpcpp.h>
#include <sys/time.h>
#include <time.h>
#include "enums.h"
#include "kvstore_client.h"
#include "warble.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReaderWriter;
using grpc::Status;

using warble::FollowReply;
using warble::FollowRequest;
using warble::ProfileReply;
using warble::ProfileRequest;
using warble::ReadReply;
using warble::ReadRequest;
using warble::RegisteruserReply;
using warble::RegisteruserRequest;
using warble::Timestamp;
using warble::Warble;
using warble::WarbleReply;
using warble::WarbleRequest;

namespace dylanwarble {

class WarbleFunctions {
 public:
  // Registers the given username
  bool RegisterUser(std::string username);

  // Posts a new warble by specific user and returns WarbleReply with id of new
  // warble
  bool PostWarble(std::string username, std::string text, std::string parent_id,
                  WarbleReply &warble_reply);

  // Lets specified user follow another user
  bool Follow(std::string username, std::string user_to_follow);

  // Reads a warble thread from the given id
  bool Read(std::string id, std::vector<Warble *> &vector_warble);

  // Returns a warble profile
  bool Profile(std::string username, std::vector<std::string> &followers,
               std::vector<std::string> &following);

  // Helper functions
 private:
  // Return true if user exists
  bool UserExists(std::string user);
  // Recursively adds a new warble to parent warble thread in kv store
  void AddWarbleToParent(std::vector<std::string> parent_ids,
                         std::string serialized_warble);
};
}  // namespace dylanwarble
