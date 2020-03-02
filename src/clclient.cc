
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
int main(int argc, char *argv[]) {
  // Breaks down the arguments, stores in a struct, and does
  // error checking
  for (int i = 0; i < argc; ++i) {
  }

  // Calls func_client event function
  return 1;
}