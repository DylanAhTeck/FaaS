#include "warble_server.h"
#include <string>

namespace dylanwarble
{

// Registers the given username
void WarbleFunctions::RegisterUser(std::string username)
{ // Create a channel with PORT 50000 and send event request

    dylanwarble::KeyValueStoreClient kvclient(grpc::CreateChannel(
        "localhost:50001", grpc::InsecureChannelCredentials()));

    Prefix kUser;
    bool success = kvclient.Put(std::to_string(kUser) + username, username);

    //Add to glog
    if (success)
        std::cout << "Register User was Succesful" << std::endl;

    else
        std::cout << "Error occured whilst registering user" << std::endl;
}

// Posts a new warble by specific user and returns WarbleReply with id of new
// warble
WarbleReply *WarbleFunctions::PostWarble(std::string username, std::string text,
                                         int reply_to_warble_id)
{
    dylanwarble::KeyValueStoreClient kvclient(grpc::CreateChannel(
        "localhost:50001", grpc::InsecureChannelCredentials()));
}

// Lets specified user follow another user
bool WarbleFunctions::Follow(std::string username, std::string user_to_follow)
{
    dylanwarble::KeyValueStoreClient kvclient(grpc::CreateChannel(
        "localhost:50001", grpc::InsecureChannelCredentials()));

    Prefix kUser;
    if (kvclient.Get(std::to_string(kUser) + username) == "" || (std::to_string(kUser) + user_to_follow == ""))
    {
        return false;
        std::cout << "User to follow or User follower does not exist" << std::endl;
    }

    //To-do - check that user isn't already following user_to_follow
    Prefix kUserFollowers;
    kvclient.Put(std::to_string(kUserFollowers) + user_to_follow, username);

    Prefix kUsersFollowing;
    kvclient.Put(std::to_string(kUsersFollowing) + username, user_to_follow);

    return true;
}

// Reads a warble thread from the given id
ReadReply *WarbleFunctions::Read(int id)
{
}

// Reads the user's profiles
std::string WarbleFunctions::Profile(std::string username)
{
    dylanwarble::KeyValueStoreClient kvclient(grpc::CreateChannel(
        "localhost:50001", grpc::InsecureChannelCredentials()));

    std::string followers;
    std::string following;

    Prefix kUserFollowers;
    Prefix kUsersFollowing;

    following = kvclient.Get(std::to_string(kUsersFollowing) + username);

    std::cerr << following << std::endl;
    return following;
}

} // namespace dylanwarble
