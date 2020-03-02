
#include "func_client.h"
#include <iostream>

namespace dylanwarble
{
// Allows service to specify that a function is valid
void FuncClient::Hook(const int event_type, const std::string &event_function)
{
  // Data we are sending to the server.
  HookRequest request;
  request.set_event_type(event_type);
  request.set_event_function(event_function);

  // Container for the data we expect from the server.
  HookReply reply;

  // Context for the client. It could be used to convey extra information to
  // the server and/or tweak certain RPC behaviors.
  ClientContext context;

  // The actual RPC.
  Status status = stub_->hook(&context, request, &reply);

  // Act upon its status.
  //TO-DO - Replace with glog
  if (status.ok())
    std::cout << "Hook SUCCESSFUL" << std::endl;
}

// Service specifies that the function is invalid
void FuncClient::Unhook(const int event_type)
{
  // Data we are sending to the server.
  UnhookRequest request;
  request.set_event_type(event_type);

  // Container for the data we expect from the server.
  UnhookReply reply;

  // Context for the client. It could be used to convey extra information to
  // the server and/or tweak certain RPC behaviors.
  ClientContext context;

  // The actual RPC.
  Status status = stub_->unhook(&context, request, &reply);

  // Act upon its status.
  if (status.ok())
    std::cout << "Unhook SUCCESSFUL" << std::endl;
}

//This takes in the struct payload and sets the requests
void FuncClient::Event(const int event_type, const Payload &p)
{

  std::cout << p.event_type << p.event_function << p.username << std::endl;

  google::protobuf::Any *payload = new google::protobuf::Any;

  switch (event_type)
  {
  case kRegisterUserID:
  {
    RegisteruserRequest request;
    request.set_username(p.username);
    payload->PackFrom(request);
    break;
  }

  case kWarbleID:
  {
    WarbleRequest request;
    request.set_username(p.username);
    request.set_text(p.text);
    request.set_parent_id(p.parent_id);
    payload->PackFrom(request);
    break;
  }

  case kFollowUserID:
  {
    FollowRequest request;
    request.set_username(p.username);
    request.set_to_follow(p.to_follow);
    payload->PackFrom(request);
    break;
  }

  case kReadID:
  {
    ReadRequest request;
    request.set_warble_id(p.id);
    payload->PackFrom(request);
    break;
  }

  case kProfileID:
  {
    ProfileRequest request;
    request.set_username(p.username);
    payload->PackFrom(request);
    break;
  }

  default:
    break;
  }

  // EventRequest.payload in this case would be one type of Warble request
  EventRequest event_request;
  event_request.set_event_type(event_type);
  event_request.set_allocated_payload(payload);

  // Container for the data we expect from the server.
  EventReply reply;

  // Context for the client. It could be used to convey extra information to
  // the server and/or tweak certain RPC behaviors.
  ClientContext context;

  // EventReply.payload in this case would be a Warble Reply

  // The actual RPC.
  Status status = stub_->event(&context, event_request, &reply);

  //Reply will now have payload if request was succesful
  google::protobuf::Any return_payload = reply.payload();

  //To-do - change this to glog
  if (!status.ok())
  {
    std::cout << "Error" << std::endl;
    return;
  }

  // If warble, return warble ID
  // If read, return warble thread
  // If profile, return followers and following

  //To-do: process and return reply message
}
} // namespace dylanwarble

// int main(int argc, char **argv)
// {
//   dylanwarble::FuncClient funcclient(grpc::CreateChannel(
//       "localhost:50000", grpc::InsecureChannelCredentials()));

//   return 0;
// }
