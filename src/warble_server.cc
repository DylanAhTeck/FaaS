// Copyright 2020 Dylan Ah Teck

#include "warble_server.h" //NOLINT
#include <string>

namespace dylanwarble {
// Helper: returns true if user already exists in key-value store
bool WarbleFunctions::UserExists(std::string user) {
  dylanwarble::KeyValueStoreClient kvclient(grpc::CreateChannel(
      "localhost:50001", grpc::InsecureChannelCredentials()));
  return !kvclient.Get(kUserKey + user).empty();
}

// Registers the given username
bool WarbleFunctions::RegisterUser(std::string username) {
  // Key-value store stub
  dylanwarble::KeyValueStoreClient kvclient(grpc::CreateChannel(
      "localhost:50001", grpc::InsecureChannelCredentials()));

  // Fail if user already exists
  if (UserExists(username)) {
    VLOG(google::ERROR)
        << "REGISTER REQUEST: User already exists. Request failed.";
    return false;
  }

  const std::string userKey = kUserKey + username;

  // Register user and return success flag
  bool success = kvclient.Put(userKey, username);
  // Add to glog
  if (success) {
    VLOG(google::INFO) << "REGISTER REQUEST: Register user was successful."
                       << std::endl;
    return true;
  }
  VLOG(google::ERROR) << "Error occured whilst registering user. " << std::endl;
  return false;
}

// Lets specified user follow another user
bool WarbleFunctions::Follow(std::string username, std::string user_to_follow) {
  // Key-value store stub
  dylanwarble::KeyValueStoreClient kvclient(grpc::CreateChannel(
      "localhost:50001", grpc::InsecureChannelCredentials()));

  // Return error if user or user to follow does not exist
  if (!UserExists(username) || !UserExists(user_to_follow)) {
    VLOG(google::ERROR) << "FOLLOW REQUEST: User or user to follow does not "
                           "exist. Request failed."
                        << std::endl;
    return false;
  }

  // Return error if user attempts to follow himself
  if (username == user_to_follow) {
    VLOG(google::ERROR)
        << "FOLLOW REQUEST: User cannot follow themselves. Request failed.";
    return false;
  }
  const std::string userFollowersKey = kFollowersKey + user_to_follow;

  // Check if user already follows user_to_follow
  std::vector<std::string> followers = kvclient.Get(userFollowersKey);
  for (int i = 0; i < followers.size(); i++) {
    if (followers[i] == username) {
      VLOG(google::ERROR) << "FOLLOW REQUEST: User is already following that "
                             "person. Request failed."
                          << std::endl;
      return false;
    }
  }

  // Add user as follower to user_to_follow
  kvclient.Put(userFollowersKey, username);

  // Add user_to_follow as someone that user follows
  const std::string userFollowingKey = kFollowingKey + username;
  kvclient.Put(userFollowingKey, user_to_follow);

  VLOG(google::INFO) << username << " began following " << user_to_follow
                     << ".";
  return true;
}

// Reads a warble thread from the given id
bool WarbleFunctions::Read(std::string id,
                           std::vector<Warble *> &warble_thread) {
  // Key-value store stub
  dylanwarble::KeyValueStoreClient kvclient(grpc::CreateChannel(
      "localhost:50001", grpc::InsecureChannelCredentials()));

  // Return error if id does exist
  if (kvclient.Get(kWarbleKey + id).empty()) {
    VLOG(google::ERROR) << "The warble does not exist. Request failed.";
    return false;
  }

  // Vector to hold the serialized warble strings
  std::vector<std::string> serialized_warbles = kvclient.Get(kWarbleKey + id);

  // Parse warble to strings
  for (int i = 0; i < serialized_warbles.size(); i++) {
    Warble *warble = new Warble;
    if (warble->ParseFromString(serialized_warbles[i]) == false) return false;
    warble_thread.push_back(warble);
  }

  return true;
}

// Posts a new warble by specific user and returns WarbleReply with id of new
// warble
bool WarbleFunctions::PostWarble(std::string username, std::string text,
                                 std::string parent_id,
                                 WarbleReply &warble_reply) {
  dylanwarble::KeyValueStoreClient kvclient(grpc::CreateChannel(
      "localhost:50001", grpc::InsecureChannelCredentials()));

  // Return false if parent_id is invalid
  if (parent_id != kStringNotSet &&
      kvclient.Get(kWarbleKey + parent_id).empty()) {
    VLOG(google::ERROR)
        << "WARBLE REQUEST: Reply ID is invalid. Request failed.";
    return false;
  }

  // Return false if user is not registered
  if (!UserExists(username)) {
    VLOG(google::ERROR)
        << "WARBLE REQUEST: User is not registered. Request failed.";
    return false;
  }

  // Default warble_id is "0"
  std::string latest_warble_id = kDefaultWarbleID;

  // If latest warble ID exists, retrieve it
  if (!kvclient.Get(kLatestWarbleID).empty()) {
    latest_warble_id = kvclient.Get(kLatestWarbleID).at(0);
  }

  // Increment latest warble ID, remove the previous, and reinsert the updated
  // one into key-value store
  // Note: a key-value store UPDATE function would be best for this
  // to guarantee concurrency. However, not sure if we are allowed to add
  // additional functions to Key-Value store API
  int latest_warble_id_as_int = stoi(latest_warble_id);
  latest_warble_id_as_int++;
  latest_warble_id = std::to_string(latest_warble_id_as_int);
  kvclient.Remove(kLatestWarbleID);
  kvclient.Put(kLatestWarbleID, latest_warble_id);

  // Create new warble and serialize
  Warble *warble = new Warble;

  // Get timestamp
  struct timeval time;
  gettimeofday(&time, NULL);
  Timestamp *timestamp = new Timestamp;
  timestamp->set_seconds(time.tv_sec);
  timestamp->set_useconds(time.tv_usec);

  // Set warble fields
  warble->set_allocated_timestamp(timestamp);
  warble->set_username(username);
  warble->set_text(text);
  warble->set_id(latest_warble_id);
  std::string serialized_warble_string;

  // If given, set parent_id and call helper to add new warble to threads of
  // parent warbles
  // Else just serialize warble
  if (parent_id != kStringNotSet) {
    warble->set_parent_id(parent_id);
    warble->SerializeToString(&serialized_warble_string);
    kvclient.Put(kWarbleParentsKey + latest_warble_id, parent_id);
    std::vector<std::string> parent_warbles =
        kvclient.Get(kWarbleParentsKey + latest_warble_id);
    AddWarbleToParent(parent_warbles, serialized_warble_string);
  } else {
    warble->SerializeToString(&serialized_warble_string);
  }

  // Add new serialized warble to database and return warble
  kvclient.Put(kWarbleKey + latest_warble_id, serialized_warble_string);
  warble_reply.set_allocated_warble(warble);

  return true;
}

// Recursive helper function to recursively add new Warble to parents
void WarbleFunctions::AddWarbleToParent(std::vector<std::string> parent_ids,
                                        std::string serialized_warble) {
  dylanwarble::KeyValueStoreClient kvclient(grpc::CreateChannel(
      "localhost:50001", grpc::InsecureChannelCredentials()));

  if (parent_ids.empty()) return;

  std::string parent_id = parent_ids[0];
  AddWarbleToParent(kvclient.Get(kWarbleParentsKey + parent_id),
                    serialized_warble);
  kvclient.Put(kWarbleKey + parent_id, serialized_warble);

  return;
}

// Reads the user's profiles
bool WarbleFunctions::Profile(std::string username,
                              std::vector<std::string> &followers,
                              std::vector<std::string> &following) {
  // Key-value store stub
  dylanwarble::KeyValueStoreClient kvclient(grpc::CreateChannel(
      "localhost:50001", grpc::InsecureChannelCredentials()));

  // Return error if user does not exist
  if (!UserExists(username)) {
    VLOG(google::ERROR) << "PROFILE REQUEST: The user does not exist."
                        << std::endl;
    return false;
  }

  // Retrieve followers and following from key-value store
  followers = kvclient.Get(kFollowersKey + username);
  following = kvclient.Get(kFollowingKey + username);

  VLOG(google::INFO) << "PROFILE REQUEST: User profile successfully returned.";
  return true;
}

}  // namespace dylanwarble
