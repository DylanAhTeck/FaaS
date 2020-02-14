
#include <fstream>
#include <iostream>

struct command
{
    std::string function;
    std::string username;
    std::string warble_text;

    int warble_id;
    int reply_warble_id;
};

/*
--registeruser <username>	Registers the given username
--user <username>		Logs in as the given username
--warble <warble text>	Creates a new warble with the given text
--reply <reply warble id>	Indicates that the new warble is a reply to the given id
--follow <username>		Starts following the given username
--read <warble id>		Reads the warble thread starting at the given id
--profile			Gets the user’s profile of following and followers
*/

int main(int argc, char *argv[])
{
    command c;
    for (int i = 0; i < argc; ++i)
    {
    }
}