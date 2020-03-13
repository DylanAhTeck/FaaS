// Copyright 2020 Dylan Ah Teck

#include <string>

namespace dylanwarble {

// Note: ignoring "For a static/global string constant,
// use a C style string instead: "const char ..."   " command

// Used as prefixes for key-value store keys
const std::string kUserKey = "USER";
const std::string kWarbleKey = "WARBLE";
const std::string kFollowersKey = "FOLLOWERS";
const std::string kFollowingKey = "FOLLOWING";
const std::string kWarbleParentsKey = "WARBLEPARENTS";

// Default Warble ID is 0
const std::string kDefaultWarbleID = "0";

// Used to retrieve the most recent warble ID
const std::string kLatestWarbleID = "WARBLEIDS";

// Used to specify an option/argument not being set by user. E.g --reply
const std::string kStringNotSet = "NOTSET";

// Used to set function names when hooking initial warble functions
const std::string kRegisterUserFunctionName = "Register user";
const std::string kWarbleFunctionName = "Warble";
const std::string kFollowFunctionName = "Follow user";
const std::string kReadFunctionName = "Read";
const std::string kProfileFunctionName = "Profile";

// For clclient
// Constants for errors in command-line argument parsing
const size_t ERROR_IN_COMMAND_LINE = 1;
const size_t SUCCESS = 0;
const size_t ERROR_UNHANDLED_EXCEPTION = 2;

// enum to store Event IDs in clear and descriptive way
enum FunctionID {
  kRegisterUserID = 0,
  kWarbleID = 1,
  kFollowUserID = 2,
  kReadID = 3,
  kProfileID = 4,
};

// enum to use in switch functions
enum FunctionName {
  kRegisterUser,
  kWarble,
  kFollowUser,
  kRead,
  kProfile,
};

}  // namespace dylanwarble
