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
  // TO-DO - Replace with glog
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

void SetRegisterUserRequest(const Payload &p, google::protobuf::Any *payload)
{
  RegisteruserRequest request;
  request.set_username(p.username);
  payload->PackFrom(request);
}

void SetWarbleRequest(const Payload &p, google::protobuf::Any *payload)
{
  WarbleRequest request;
  request.set_username(p.username);
  request.set_text(p.text);
  request.set_parent_id(p.parent_id);
  payload->PackFrom(request);
}

void SetFollowRequest(const Payload &p, google::protobuf::Any *payload)
{
  FollowRequest request;
  request.set_username(p.username);
  request.set_to_follow(p.to_follow);
  payload->PackFrom(request);
}
void SetReadRequest(const Payload &p, google::protobuf::Any *payload)
{
  ReadRequest request;
  request.set_warble_id(p.id);
  payload->PackFrom(request);
}
void SetProfileRequest(const Payload &p, google::protobuf::Any *payload)
{
  ProfileRequest request;
  request.set_username(p.username);
  payload->PackFrom(request);
}

//Change return variable to be a function

// This takes in the struct payload and sets the requests
void FuncClient::Event(const int event_type, const Payload &p, CommandResponse &r)
{
  google::protobuf::Any *payload = new google::protobuf::Any;

  switch (event_type)
  {
  case kRegisterUserID:
    SetRegisterUserRequest(p, payload);
    break;

  case kWarbleID:
    SetWarbleRequest(p, payload);
    break;

  case kFollowUserID:
    SetFollowRequest(p, payload);
    break;

  case kReadID:
    SetReadRequest(p, payload);
    break;

  case kProfileID:
    SetProfileRequest(p, payload);
    break;

  default:
    std::cerr << "Invalid event_type used in Event function" << std::endl;
    return;
  }

  // EventRequest.payload in this case would be one type of Warble request
  EventRequest event_request;
  event_request.set_event_type(event_type);
  event_request.set_allocated_payload(payload);

  // Container for the data we expect from the server.
  EventReply event_reply;

  // Context for the client. It could be used to convey extra information to
  // the server and/or tweak certain RPC behaviors.
  ClientContext context;

  // EventReply.payload in this case would be a Warble Reply

  // The actual RPC.
  Status status = stub_->event(&context, event_request, &event_reply);

  // Reply will now have payload if request was succesful
  google::protobuf::Any return_payload = event_reply.payload();

  // To-do - change this to glog
  if (!status.ok())
  {
    std::cout << "Func service failed. Please use a valid command." << std::endl;
    return;
  }

  switch (event_type)
  {
  case kRegisterUserID:
    r.success = true;
    return;
    break;

  case kWarbleID:
    r.success = true;
    return;
    break;

  case kFollowUserID:
    r.success = true;
    return;
    break;

    case kReadID:
      r.success = true;
      return;
      break;

  case kProfileID:
    // dylanwarble::ProfileReply reply;
    // return_payload.UnpackTo(&reply);
    // r.followers = reply.followers();
    // r.following = reply.following();
    break;

  default:
    std::cerr << "Invalid event_type used in Event function" << std::endl;
    return;
  }

  // If warble, return warble ID
  // If read, return warble thread
  // If profile, return followers and following

  // To-do: process and return reply message
}
} // namespace dylanwarble
