#include "warble_server.h"
#include <string>

namespace dylanwarble
{

// Registers the given username
bool WarbleFunctions::RegisterUser(std::string username)
{ // Create a channel with PORT 50000 and send event request

    // dylanwarble::KeyValueStoreClient kvclient(grpc::CreateChannel(
    //     "localhost:50001", grpc::InsecureChannelCredentials()));

    // //Prefix kUser;
    // const std::string userKey = "USER" + username;
    // bool success = kvclient.Put(userKey, username);

    // if (success)
    // {
    //     std::cout << "Register User was Succesful" << std::endl;
    //     return true;
    // }

    // std::cout << "Error occured whilst registering user" << std::endl;
    // return false;
}

// Lets specified user follow another user
bool WarbleFunctions::Follow(std::string username, std::string user_to_follow)
{
    // dylanwarble::KeyValueStoreClient kvclient(grpc::CreateChannel(
    //     "localhost:50001", grpc::InsecureChannelCredentials()));

    // //Prefix kUser;
    // const std::string USER = "USER";
    // if (kvclient.Get(USER + username) == "" || (kvclient.Get(USER + user_to_follow) == ""))
    // {
    //     std::cout << "User to follow or User follower does not exist" << std::endl;
    //     return false;
    // }

    // //To-do - check that user isn't already following user_to_follow
    // Prefix kUserFollowers;
    // const std::string userFollowersKey = "FOLLOWERS" + user_to_follow;
    // //db.Put(std::to_string(kUserFollowers) + user_to_follow, username);
    // kvclient.Put(userFollowersKey, username);

    // Prefix kUserFollowing;
    // const std::string userFollowingKey = "FOLLOWING" + username;
    // //db.Put(std::to_string(kUserFollowers) + user_to_follow, username);
    // kvclient.Put(userFollowersKey, user_to_follow);

    // std::cout << username << " began following " << user_to_follow << std::endl;
    // return true;
}

// Reads a warble thread from the given id
bool WarbleFunctions::Read(std::string id, Warble &warble)
{
    // dylanwarble::KeyValueStoreClient kvclient(grpc::CreateChannel(
    //     "localhost:50001", grpc::InsecureChannelCredentials()));

    // if (kvclient.Get(WARBLETHREAD + id) == "")
    //     return false;
}

// Posts a new warble by specific user and returns WarbleReply with id of new
// warble
bool WarbleFunctions::PostWarble(std::string username, std::string text,
                                 int reply_to_warble_id, WarbleReply &warble_reply)
{
    // dylanwarble::KeyValueStoreClient kvclient(grpc::CreateChannel(
    //     "localhost:50001", grpc::InsecureChannelCredentials()));

    // const std::string warbleids = "WARBLEIDS";

    // std::string id = kvclient.Get(warbleids);

    // // If id is empty string, means this is the first warble
    // int integer_id = 0;
    // if (id != "")
    //     integer_id = stoi(id);

    // integer_id++;
    // id = std::to_string(integer_id);
    // kvclient.Remove(warbleids);
    // kvclient.Put(warbleids, id);

    // // const std::string new_warble = "WARBLE" + id;
    // // kvclient.Put(new_warble, text);

    // Warble warble;
    // warble.set_username(username);
    // warble.set_text(text);
    // warble.set_id(id);
    // if (reply_to_warble_id != -1)
    //     warble.set_parent_id(std::to_string(reply_to_warble_id));

    // std::string value;
    // warble.SerializeToString(&value);

    // kvclient.Put("WARBLE" + id, value);

    //serialized to string ->
    // if (reply_to_warble_id != 0)
    // {
    //     const std::string reply_warble_key = "WARBLE" + reply_to_warble_id;
    //     kvclient.Put(new_warble, text);
    // }
    return true;
}

// Reads the user's profiles
std::string WarbleFunctions::Profile(std::string username)
{
    // dylanwarble::KeyValueStoreClient kvclient(grpc::CreateChannel(
    //     "localhost:50001", grpc::InsecureChannelCredentials()));

    // std::string followers;
    // std::string following;

    // std::string userFollowersKey = "FOLLOWERS" + username;
    // std::string userFollowingKey = "FOLLOWING" + username;

    // following = kvclient.Get(userFollowersKey);

    // std::cerr << "HERE: " << following << std::endl;

    // return following;
}

} // namespace dylanwarble
