// Copyright 2020 Dylan Ah Teck

#include "func_client.h"  // NOLINT

#include <grpcpp/grpcpp.h>            // NOLINT
#include <boost/program_options.hpp>  // NOLINT

#include <fstream>   // NOLINT
#include <iostream>  // NOLINT

/*
Event type
0                --registeruser <username>	Registers the given username
                 --user <username>		Logs in as the given username
1                --warble <warble text>	Creates a new warble with the given text
                 --reply <reply warble id> Indicates that the new warble is a
reply to the given id 2                --follow <username> Starts following the
given username 3                --read <warble id> Reads the warble thread
starting at the given id 4                --profile Gets the user’s profile of
following and followers
*/

/*
General Flow:

1) The user inputs a valid command on the command line
2) After error checking, the clclient stores all the
information in a Payload and calls the func_client event function
3) The func_client sets the requests and sends to func_server using
stubs
4) Func_server unpacks the Func requests (to get a Warble request)
and calls the appropriate warble function
5) The warble function does the logic and stores/updates data
on key-value
6) The func_server returns to func_client. Func_client
sets a CommandResponse with reply values and sends that to clcclient
7) Clclient uses CommandResponse to print appropriate messages
*/

namespace dylanwarble {

// Helper functions to process CommandResponse

// Prints success or failure message after attempting to register user
void ProcessRegisterUserReply(struct Payload *p,
                              const struct CommandResponse *cr) {
  if (cr->success == true) {
    std::cout << "User '" << p->username << "' was succesfully registered."
              << std::endl;
  } else {
    std::cout << "The user " << p->username
              << " already exists and could not be registered." << std::endl;
  }

  // Deallocate Payload and CommandResponse
  delete cr;
  delete p;
}

// Displays content of warble if successfull, else prints error message
void ProcessWarbleReply(struct Payload *p, const struct CommandResponse *cr) {
  if (cr->success == true) {
    std::cout << "Your new warble (ID #" << cr->warbleID
              << ") was successfully posted." << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
    std::cout << "Warble ID #" << cr->warbleID << ")" << std::endl;
    if (!cr->parent_id.empty())
      std::cout << "Parent ID #" << cr->reply_id << ")" << std::endl;
    std::cout << "User: " << cr->username << std::endl;
    std::cout << "Text: " << cr->warble_text << std::endl;
    std::cout << "Timestamp: " << cr->timestamp_seconds << " seconds, "
              << cr->timestamp_u_seconds << " milliseconds" << std::endl;
  } else {
    std::cout << "Your new warble could not be posted." << std::endl;
  }

  // Deallocate Payload and CommandResponse
  delete cr;
  delete p;
}

// Prints success or failure message after attempting to follow another user
void ProcessFollowUserRequest(struct Payload *p,
                              const struct CommandResponse *cr) {
  if (cr->success == true) {
    std::cout << "User '" << p->username << "' successfully started following '"
              << p->to_follow << "'" << std::endl;
  } else {
    std::cout << "User " << p->username
              << "'s follow request could not be processed." << std::endl;
  }

  // Deallocate Payload and CommandResponse
  delete cr;
  delete p;
}

// Prints warble thread or prints error message
void ProcessReadRequest(struct Payload *p, const struct CommandResponse *cr) {
  if (cr->success == true) {
    std::cout << "You have successfully read the warble thread starting at ID #"
              << p->id << ":" << std::endl;
    for (int i = 0; i < cr->warble_threads.size(); i++) {
      std::cout << "-----------------------------------------" << std::endl;
      std::cout << "Warble ID #" << cr->warble_threads[i].id() << ")"
                << std::endl;
      if (!cr->warble_threads[i].parent_id().empty())
        std::cout << "Parent ID #" << cr->warble_threads[i].parent_id() << ")"
                  << std::endl;
      std::cout << "User: " << cr->warble_threads[i].username() << std::endl;
      std::cout << "Warble: " << cr->warble_threads[i].text() << ""
                << std::endl;
      std::cout << "Timestamp: " << cr->warble_threads[i].timestamp().seconds()
                << " seconds" << std::endl;
    }
  } else {
    std::cout << "User " << p->username
              << "'s request to read warble could not be processed."
              << std::endl;
  }

  // Deallocate Payload and CommandResponse
  delete cr;
  delete p;
}

// Prints the list of user's followers and following
// If no followers or no following, the list will simply be printed with no
// users shown
void ProcessProfileRequest(struct Payload *p,
                           const struct CommandResponse *cr) {
  if (cr->success == true) {
    std::cout << "You have successfully found User " << p->username
              << "'s profile." << std::endl;
    std::cout << "Followers:" << std::endl;
    for (int i = 0; i < cr->followers.size(); i++) {
      std::cout << i << ") " << cr->followers[i] << std::endl;
    }
    std::cout << "Following" << std::endl;
    for (int i = 0; i < cr->following.size(); i++) {
      std::cout << i << ") " << cr->following[i] << std::endl;
    }
  } else {
    std::cout << "User " << p->username << "'s profile could not be processed."
              << std::endl;
  }
  // Deallocate Payload and CommandResponse
  delete cr;
  delete p;
}

// Takes in a CommandResponse and processes the data inside to print to
// commandline and inform user
void ProcessReply(int event_type, struct Payload *p,
                  const struct CommandResponse *cr) {
  switch (event_type) {
    case kRegisterUserID: {
      ProcessRegisterUserReply(p, cr);
      break;
    }
    case kWarbleID: {
      ProcessWarbleReply(p, cr);
      break;
    }
    case kFollowUserID: {
      ProcessFollowUserRequest(p, cr);
      break;
    }
    case kReadID: {
      ProcessReadRequest(p, cr);
      break;
    }
    case kProfileID: {
      ProcessProfileRequest(p, cr);
      break;
    }

    default:
      std::cout << "Error processing reply." << std::endl;
  }
}

// Function takes a set payload, creates a funcclient stub and calls event
// function
void SendPayload(int event_type, struct Payload *p) {
  // Create a channel with PORT 50000 and send event request
  FuncClient funcclient(grpc::CreateChannel(
      "localhost:50000", grpc::InsecureChannelCredentials()));

  CommandResponse *res = new CommandResponse;
  funcclient.Event(event_type, p, res);
  ProcessReply(event_type, p, res);
}

// Function to set values of payload
// Kept as a single function as opposed to process replies
// as this is a smaller function
void SetPayload(struct Payload *p, int event_type,
                const boost::program_options::variables_map &vm) {
  switch (event_type) {
    // Register user
    case kRegisterUserID: {
      p->event_type = kRegisterUserID;
      p->event_function = kRegisterUser;
      p->username = vm["registeruser"].as<std::string>();
      break;
    }
    // Warble (optional reply)
    case kWarbleID: {
      p->event_type = kWarbleID;
      p->event_function = kWarble;
      p->username = vm["user"].as<std::string>();
      std::vector<std::string> warble_vector =
          vm["warble"].as<std::vector<std::string>>();
      for (int i = 0; i < warble_vector.size(); i++) {
        if (i != 0)
          p->text = p->text + " " + warble_vector[i];
        else
          p->text = p->text + warble_vector[i];
      }
      if (vm.count("reply"))
        p->parent_id = vm["reply"].as<std::string>();
      else
        p->parent_id = kStringNotSet;
      break;
    }

    // Follow user
    case kFollowUserID: {
      p->event_type = kFollowUserID;
      p->event_function = kFollowUser;
      p->username = vm["user"].as<std::string>();
      p->to_follow = vm["follow"].as<std::string>();
      break;
    }

    // Read warble ID
    case kReadID: {
      p->event_type = kReadID;
      p->event_function = kRead;
      p->username = vm["user"].as<std::string>();
      p->id = vm["read"].as<std::string>();
      break;
    }

    // Get followers and following
    case kProfileID: {
      p->event_type = kProfileID;
      p->event_function = kProfile;
      p->username = vm["user"].as<std::string>();
      break;
    }
    default:
      std::cout << "Invalid event type" << std::endl;
  }
}

// Helper function to effectuate basic error checking of command line
// Returns true if valid
bool ValidCommand(const boost::program_options::variables_map &vm) {
  // Checks if arguments are appropriate as per each function definition
  if (vm.count("registeruser") && vm.size() > 1 ||
      vm.count("warble") && !vm.count("reply") && vm.size() > 2 ||
      vm.count("warble") && vm.count("reply") && vm.size() > 3 ||
      vm.count("read") && vm.size() > 2 ||
      vm.count("profile") && vm.size() > 2 ||
      vm.count("follow") && vm.size() > 2) {
    std::cout << "Invalid: too many commands." << std::endl;
    return false;
  }

  // If not register user function, must specify a user
  if (!vm.count("user") && !vm.count("registeruser")) {
    std::cerr << "Invalid: command requires a specified user" << std::endl;
    return false;
  }

  return true;
}

}  // namespace dylanwarble

int main(int argc, char *argv[]) {
  dylanwarble::Payload *payload = new dylanwarble::Payload;
  namespace po = boost::program_options;

  // Initialize Google's logging library.
  google::InitGoogleLogging(argv[0]);
  // Set FLAGS_logtostderr automatically
  FLAGS_logtostderr = 1;

  try {
    // Use boost library to parse command-line
    po::options_description desc{"Options"};

    // Add possible flags
    desc.add_options()("registeruser", po::value<std::string>(),
                       "Register User")("user", po::value<std::string>(),
                                        "User")(
        "warble", po::value<std::vector<std::string>>()->multitoken(),
        "Warble")("reply", po::value<std::string>(), "Reply Warble ID")(
        "follow", po::value<std::string>(), "Username")(
        "read", po::value<std::string>(), "Read")("profile", "Profile");

    // Stores flag arguments in a map
    po::variables_map vm;
    po::store(parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    // Basic error checking done in helper function
    if (!dylanwarble::ValidCommand(vm)) {
      delete payload;
      return dylanwarble::ERROR_IN_COMMAND_LINE;
    }

    // Valid command as from here
    if (vm.count("registeruser")) {
      SetPayload(payload, dylanwarble::kRegisterUserID, vm);
    } else if (vm.count("follow")) {
      SetPayload(payload, dylanwarble::kFollowUserID, vm);
    } else if (vm.count("warble")) {
      SetPayload(payload, dylanwarble::kWarbleID, vm);
    } else if (vm.count("read")) {
      SetPayload(payload, dylanwarble::kReadID, vm);
    } else if (vm.count("profile")) {
      SetPayload(payload, dylanwarble::kProfileID, vm);
    }
    // Send payload once set
    SendPayload(payload->event_type, payload);
  }

  // Catches exception thrown when using boost program options
  catch (po::error &ex) {
    delete payload;
    std::cerr << ex.what() << '\n';
    return dylanwarble::ERROR_UNHANDLED_EXCEPTION;
  }

  return dylanwarble::SUCCESS;
}
