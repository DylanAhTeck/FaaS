
#include <fstream>
#include <iostream>
#include <boost/program_options.hpp>
#include <grpcpp/grpcpp.h>
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
>>>>>>> Stashed changes

    std::string username;
    std::string text;
    std::string parent_id;
    std::string id;
    std::string to_follow;
};
*/

namespace
{
const size_t ERROR_IN_COMMAND_LINE = 1;
const size_t SUCCESS = 0;
const size_t ERROR_UNHANDLED_EXCEPTION = 2;
} // namespace

void SetPayload(struct Payload &p, int event_type, const boost::program_options::variables_map &vm)
{

  switch (event_type)
  {
  //Register user
  case 0:
    p.event_type = 0;
    p.event_function = "registeruser";
    p.username = vm["registeruser"].as<std::string>();
    break;

  //Warble (optional reply)
  case 1:
    p.event_type = 1;
    p.event_function = "warble";
    p.username = vm["user"].as<std::string>();
    p.text = vm["warble"].as<std::string>();
    if (vm.count("reply"))
      p.parent_id = vm["reply"].as<std::string>();
    break;

  //Follow user
  case 2:
    p.event_type = 2;
    p.event_function = "follow";
    p.username = vm["username"].as<std::string>();
    p.to_follow = vm["follow"].as<std::string>();
    break;

  //Read warble ID
  case 3:
    p.event_type = 3;
    p.event_function = "read";
    p.username = vm["user"].as<std::string>();
    p.id = vm["read"].as<std::string>();
    break;

  //Get followers and following
  case 4:
    p.event_type = 4;
    p.event_function = "profile";
    p.username = vm["user"].as<std::string>();
    break;

  default:
    std::cerr << "Invalid event type" << std::endl;
  }

  FuncClient funcclient(grpc::CreateChannel(
      "localhost:50000", grpc::InsecureChannelCredentials()));

  funcclient.event(event_type, p);
}

int main(int argc, char *argv[])
{
  Payload command;
  bool too_many_commands = false;
  namespace po = boost::program_options;
  try
  {

    po::options_description desc{"Options"};
    desc.add_options()("registeruser", po::value<std::string>(), "Register User")("user", po::value<std::string>(), "User")("warble", po::value<std::string>(), "Warble")("reply", po::value<std::string>(), "Reply Warble ID")("follow", po::value<std::string>(), "Username")("read", po::value<std::string>(), "Read")("profile", "Profile");

    po::variables_map vm;
    po::store(parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    //Error checking
    if (vm.count("registeruser") && vm.size() > 1 ||
        vm.count("warble") && !vm.count("reply") && vm.size() > 2 ||
        vm.count("warble") && vm.count("reply") && vm.size() > 3 ||
        vm.count("read") && vm.size() > 2 ||
        vm.count("profile") && vm.size() > 2 ||
        vm.count("follow") && vm.size() > 2)
    {
      std::cerr << "Error: too many commands" << std::endl;
      return ERROR_IN_COMMAND_LINE;
    }

    if (!vm.count("user") && !vm.count("registeruser"))
    {
      std::cerr << "Error: command requires a specified user" << std::endl;
      return ERROR_IN_COMMAND_LINE;
    }

    //Valid command as from here
    if (vm.count("registeruser"))
      SetPayload(command, 0, vm);
    else if (vm.count("warble"))
      SetPayload(command, 1, vm);
    else if (vm.count("follow"))
      SetPayload(command, 2, vm);
    else if (vm.count("read"))
      SetPayload(command, 3, vm);
    else if (vm.count("profile"))
      SetPayload(command, 4, vm);
  }
  catch (po::error &ex)
  {
    std::cerr << ex.what() << '\n';
    return ERROR_UNHANDLED_EXCEPTION;
  }

  return SUCCESS;
}
