
#include <grpcpp/grpcpp.h>
#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>
#include "func_client.h"
/*
Event type
0                --registeruser <username>	Registers the given username
                 --user <username>		Logs in as the given username
1                --warble <warble text>	Creates a new warble with the given text
                 --reply <reply warble id>	Indicates that the new warble is
a reply to the given id 2                --follow <username>
Starts following the given username 3                --read <warble id>
Reads the warble thread starting at the given id 4                --profile
Gets the user’s profile of following and followers
*/

/*
General Flow:

1) The user inputs a valid command on the command line
2) After error checking, the clclient stores all the
information in a struct and calls the func_client event function
3) The func_client sets the requests and sends to func_server using
stubs
4) Func_server unpacks the Func requests (to get a Warble request)
and calls the appropriate warble function
5) The warble function does the logic and stores/updates data
on kv_store (an instance of which lives in func instance)
6) The func_server returns to func_client. Func_client

unpacks the reply and displays it on terminal
*/

/* Defined in func_client.h
struct Payload : google::protobuf::Message
{
    int event_type;
    std::string event_function;

    std::string username;
    std::string text;
    std::string parent_id;
    std::string id;
    std::string to_follow;
};
*/

namespace dylanwarble
{

// Constants for Errors in command-line argument parsing
const size_t ERROR_IN_COMMAND_LINE = 1;
const size_t SUCCESS = 0;
const size_t ERROR_UNHANDLED_EXCEPTION = 2;

void ProcessReply(int event_type, struct Payload &p, struct CommandResponse &cr)
{
  switch (event_type)
  {
  case kRegisterUserID:
  {
    if (cr.success == true)
      std::cout << "User '" << p.username << "' was succesfully registered." << std::endl;
    else
      std::cout << "User " << p.username << " could not be registered." << std::endl;
    break;
  }

  case kWarbleID:
  {
    if (cr.success == true)
      std::cout << "Your new warble (ID #" << cr.warbleID << ") was successfully posted." << std::endl;
    else
      std::cout << "Your new warble could not be posted." << std::endl;
    break;
  }

  // Follow user
  case kFollowUserID:
  {
    if (cr.success == true)
      std::cout << "User '" << p.username << "'succesfully started following '" << p.to_follow << "'" << std::endl;
    else
      std::cout << "User " << p.username << "'s follow request could not be processed." << std::endl;
    break;
  }

  // Read warble ID
  case kReadID:
  {
    if (cr.success == true)
      std::cout << "You have successfully read the warble." << std::endl;
    else
      std::cout << "User " << p.username << "'s request to read warble could not be processed." << std::endl;
    break;
  }
  break;

  // Get followers and following
  case kProfileID:
  {
    if (cr.success == true)
    {
      std::vector<std::string> followers = cr.followers;
      std::vector<std::string> following = cr.following;

      std::cout << "User '" << p.username << "' is following:" << std::endl;
      for (int i = 0; i < cr.following.size(); i++)
        std::cout << i << ". " << following[i] << std::endl;
      std::cout << "User '" << p.username << "' has followers:" << std::endl;
      for (int i = 0; i < cr.followers.size(); i++)
        std::cout << i << ". " << followers[i] << std::endl;
    }
    else
      std::cout << "User '" << p.username << "'s profile could not be processed." << std::endl;
    break;
  }

  default:
    std::cerr << "Error processing reply" << std::endl;
  }
}

void SendPayload(int event_type, struct Payload &p)
{
  // Create a channel with PORT 50000 and send event request
  FuncClient funcclient(grpc::CreateChannel(
      "localhost:50000", grpc::InsecureChannelCredentials()));

  CommandResponse res;
  funcclient.Event(event_type, p, res);
  ProcessReply(event_type, p, res);
}

// Function to set payload and send to func_client
void SetPayload(struct Payload &p, int event_type,
                const boost::program_options::variables_map &vm)
{
  switch (event_type)
  {
  // Register user
  case kRegisterUserID:
    p.event_type = kRegisterUserID;
    p.event_function = kRegisterUser;
    p.username = vm["registeruser"].as<std::string>();
    break;

  // Warble (optional reply)
  case kWarbleID:
    p.event_type = kWarbleID;
    p.event_function = kWarble;
    p.username = vm["user"].as<std::string>();
    p.text = vm["warble"].as<std::string>();
    if (vm.count("reply"))
      p.parent_id = vm["reply"].as<std::string>();
    break;

  // Follow user
  case kFollowUserID:
    p.event_type = kFollowUserID;
    p.event_function = kFollowUser;
    p.username = vm["user"].as<std::string>();
    p.to_follow = vm["follow"].as<std::string>();
    break;

  // Read warble ID
  case kReadID:
    p.event_type = kReadID;
    p.event_function = kRead;
    p.username = vm["user"].as<std::string>();
    p.id = vm["read"].as<std::string>();
    break;

  // Get followers and following
  case kProfileID:
    p.event_type = kProfileID;
    p.event_function = kProfile;
    p.username = vm["user"].as<std::string>();
    break;

  default:
    std::cerr << "Invalid event type" << std::endl;
  }

  SendPayload(event_type, p);
}

} // namespace dylanwarble

int main(int argc, char *argv[])
{
  dylanwarble::Payload command;
  bool too_many_commands = false;
  namespace po = boost::program_options;

  try
  {
    // Use boost library to parse command-line
    po::options_description desc{"Options"};

    // Add possible flags
    desc.add_options()("registeruser", po::value<std::string>(),
                       "Register User")("user", po::value<std::string>(),
                                        "User")(
        "warble", po::value<std::string>(), "Warble")(
        "reply", po::value<std::string>(), "Reply Warble ID")(
        "follow", po::value<std::string>(), "Username")(
        "read", po::value<std::string>(), "Read")("profile", "Profile");

    // Stores flag arguments in a map
    po::variables_map vm;
    po::store(parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    // Basic error checking
    if (vm.count("registeruser") && vm.size() > 1 ||
        vm.count("warble") && !vm.count("reply") && vm.size() > 2 ||
        vm.count("warble") && vm.count("reply") && vm.size() > 3 ||
        vm.count("read") && vm.size() > 2 ||
        vm.count("profile") && vm.size() > 2 ||
        vm.count("follow") && vm.size() > 2)
    {
      std::cerr << "Error: too many commands" << std::endl;
      return dylanwarble::ERROR_IN_COMMAND_LINE;
    }

    if (!vm.count("user") && !vm.count("registeruser"))
    {
      std::cerr << "Error: command requires a specified user" << std::endl;
      return dylanwarble::ERROR_IN_COMMAND_LINE;
    }

    // Valid command as from here
    if (vm.count("registeruser"))
      SetPayload(command, dylanwarble::kRegisterUserID, vm);
    else if (vm.count("follow"))
      SetPayload(command, dylanwarble::kFollowUserID, vm);
    else if (vm.count("warble"))
      SetPayload(command, dylanwarble::kWarbleID, vm);
    else if (vm.count("read"))
      SetPayload(command, dylanwarble::kReadID, vm);
    else if (vm.count("profile"))
      SetPayload(command, dylanwarble::kProfileID, vm);
  }
  catch (po::error &ex)
  {
    std::cerr << ex.what() << '\n';
    return dylanwarble::ERROR_UNHANDLED_EXCEPTION;
  }

  return dylanwarble::SUCCESS;
}
