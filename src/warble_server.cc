#include "warble_server.h"
#include <string>

namespace dylanwarble
{

// Registers the given username
bool WarbleFunctions::RegisterUser(std::string username)
{ // Create a channel with PORT 50000 and send event request

    dylanwarble::KeyValueStoreClient kvclient(grpc::CreateChannel(
        "localhost:50001", grpc::InsecureChannelCredentials()));

    //Prefix kUser;
    const std::string userKey = "USER" + username;
    bool success = kvclient.Put(userKey, username);

    if (success)
    {
        std::cout << "Register User was Succesful" << std::endl;
        return true;
    }

    std::cout << "Error occured whilst registering user" << std::endl;
    return false;
}

// Lets specified user follow another user
bool WarbleFunctions::Follow(std::string username, std::string user_to_follow)
{
    dylanwarble::KeyValueStoreClient kvclient(grpc::CreateChannel(
        "localhost:50001", grpc::InsecureChannelCredentials()));

    //Prefix kUser;
    const std::string USER = "USER";
    if (kvclient.Get(USER + username) == "" || (kvclient.Get(USER + user_to_follow) == ""))
    {
        std::cout << "User to follow or User follower does not exist" << std::endl;
        return false;
    }

    //To-do - check that user isn't already following user_to_follow
    Prefix kUserFollowers;
    const std::string userFollowersKey = "FOLLOWERS" + user_to_follow;
    //db.Put(std::to_string(kUserFollowers) + user_to_follow, username);
    kvclient.Put(userFollowersKey, username);

    Prefix kUserFollowing;
    const std::string userFollowingKey = "FOLLOWING" + username;
    //db.Put(std::to_string(kUserFollowers) + user_to_follow, username);
    kvclient.Put(userFollowersKey, user_to_follow);

    std::cout << username << " began following " << user_to_follow << std::endl;
    return true;
}

// Reads a warble thread from the given id
ReadReply *WarbleFunctions::Read(int id)
{
}

// Posts a new warble by specific user and returns WarbleReply with id of new
// warble
WarbleReply *WarbleFunctions::PostWarble(std::string username, std::string text,
                                         int reply_to_warble_id)
{
    dylanwarble::KeyValueStoreClient kvclient(grpc::CreateChannel(
        "localhost:50001", grpc::InsecureChannelCredentials()));
}

// Reads the user's profiles
std::string WarbleFunctions::Profile(std::string username)
{
    dylanwarble::KeyValueStoreClient kvclient(grpc::CreateChannel(
        "localhost:50001", grpc::InsecureChannelCredentials()));

    std::string followers;
    std::string following;

    std::string userFollowersKey = "FOLLOWERS" + username;
    std::string userFollowingKey = "FOLLOWING" + username;

    following = kvclient.Get(userFollowersKey);

    std::cerr << "HERE: " << following << std::endl;

    return following;
}

} // namespace dylanwarble
