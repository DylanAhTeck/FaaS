
namespace dylanwarble
{

// enum for event_id
enum FunctionID
{
    kRegisterUserID = 0,
    kWarbleID = 1,
    kFollowUserID = 2,
    kReadID = 3,
    kProfileID = 4,
};

// enum for event
enum FunctionName
{
    kRegisterUser,
    kWarble,
    kFollowUser,
    kRead,
    kProfile,
};

enum Prefix
{
    kUser = 0,
    kUserFollowers = 1,
    kUsersFollowing = 2,

};

enum Checks
{

};

} // namespace dylanwarble