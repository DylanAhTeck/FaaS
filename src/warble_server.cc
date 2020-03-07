#include "warble_server.h"
#include <grpcpp/grpcpp.h>

// Registers the given username
void Warble::registeruser(std::string username) {}

// Posts a new warble by specific user and returns WarbleReply with id of new
// warble
WarbleReply *Warble::warble(std::string username, std::string text,
                            int reply_to_warble_id) {}

// Lets specified user follow another user
FollowReply *Warble::follow(std::string username, std::string user_to_follow) {}

// Reads a warble thread from the given id
ReadReply *Warble::read(int id) {}

//
ProfileReply *Warble::profile(std::string username) {}

// How do I compile and link warble_server and warble.grpc.pb.h
// without a main function?
int main() { return 1; }