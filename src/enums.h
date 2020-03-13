
namespace dylanwarble {

// Constants for Errors in command-line argument parsing
const std::string kUserKey = "USER";
const std::string kLatestWarbleID = "WARBLEIDS";
const std::string kWarbleKey = "WARBLE";
const std::string kWarbleThread = "WARBLETHREAD";
const std::string kFollowersKey = "FOLLOWERS";
const std::string kFollowingKey = "FOLLOWING";
const std::string kWarbleParents = "WARBLEPARENTS";
const std::string kDefaultWarbleID = "0";

const int kNotSet = -1;

// For clclient
// Constants for errors in command-line argument parsing
const size_t ERROR_IN_COMMAND_LINE = 1;
const size_t SUCCESS = 0;
const size_t ERROR_UNHANDLED_EXCEPTION = 2;

const std::string kStringNotSet = "NOTSET";

// enum for event_id
enum FunctionID {
  kRegisterUserID = 0,
  kWarbleID = 1,
  kFollowUserID = 2,
  kReadID = 3,
  kProfileID = 4,
};

// enum for event
enum FunctionName {
  kRegisterUser,
  kWarble,
  kFollowUser,
  kRead,
  kProfile,
};

}  // namespace dylanwarble