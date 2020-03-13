#include "func_client.h"
#include <glog/logging.h>
#include <iostream>

namespace dylanwarble {

// Allows service to specify that a function is valid
bool FuncClient::Hook(const int event_type, const std::string &event_function) {
  // Data we are sending to the server.
  HookRequest request;
  request.set_event_type(event_type);
  request.set_event_function(event_function);

  // Container for the data we expect from the server.
  HookReply reply;

  // Context for the client.
  ClientContext context;

  // The actual RPC.
  Status status = stub_->hook(&context, request, &reply);

  // Act upon its status.
  if (status.ok()) {
    VLOG(google::INFO) << "HOOK REQUEST: Successful." << std::endl;
    return true;
  } else {
    VLOG(google::ERROR) << "HOOK REQUEST: Failed." << std::endl;
    return false;
  }
}

// Allows service to specify that the function of event_type is invalid
bool FuncClient::Unhook(const int event_type) {
  // Data we are sending to the server.
  UnhookRequest request;
  request.set_event_type(event_type);

  // Container for the data we expect from the server.
  UnhookReply reply;

  // Context for the client.
  ClientContext context;

  // The actual RPC.
  Status status = stub_->unhook(&context, request, &reply);

  // Act upon its status.
  if (status.ok()) {
    VLOG(google::INFO) << "UNHOOK REQUEST: Successful." << std::endl;
    return true;
  } else {
    VLOG(google::ERROR) << "UNHOOK REQUEST: Failed." << std::endl;
    return false;
  }
}

// TO SEND TO FUNC_SERVER - Helper functions to help set parameters for Warble
// request messages and pack in a google protobuf
void SetRegisterUserRequest(const Payload *p, google::protobuf::Any *payload) {
  RegisteruserRequest request;
  request.set_username(p->username);
  payload->PackFrom(request);
}

void SetWarbleRequest(const Payload *p, google::protobuf::Any *payload) {
  WarbleRequest request;
  request.set_username(p->username);
  request.set_text(p->text);
  request.set_parent_id(p->parent_id);
  payload->PackFrom(request);
}

void SetFollowRequest(const Payload *p, google::protobuf::Any *payload) {
  FollowRequest request;
  request.set_username(p->username);
  request.set_to_follow(p->to_follow);
  payload->PackFrom(request);
}
void SetReadRequest(const Payload *p, google::protobuf::Any *payload) {
  ReadRequest request;
  request.set_warble_id(p->id);
  payload->PackFrom(request);
}
void SetProfileRequest(const Payload *p, google::protobuf::Any *payload) {
  ProfileRequest request;
  request.set_username(p->username);
  payload->PackFrom(request);
}

// TO SEND TO CL_CLIENT - Helper functions to unpack reply messages
// and store Warble message data in a CommandResponse struct

void SetRegisterUserReply(CommandResponse *r,
                          const google::protobuf::Any &return_payload) {
  r->success = true;
  return;
}

void SetWarbleReply(CommandResponse *r, const google::protobuf::Any &return_payload) {
  WarbleReply reply;

  return_payload.UnpackTo(&reply);
  r->success = true;

  r->warbleID = reply.warble().id();

  r->warble_text = reply.warble().text();

  r->reply_id = reply.warble().parent_id();
  r->username = reply.warble().username();

  Timestamp timestamp = reply.warble().timestamp();
  r->timestamp_seconds = timestamp.seconds();
  r->timestamp_u_seconds = timestamp.useconds();

  return;
}

void SetFollowUserReply(CommandResponse *r,
                        const google::protobuf::Any &return_payload) {
  r->success = true;
  return;
}

void SetReadReply(CommandResponse *r, const google::protobuf::Any &return_payload) {
  ReadReply reply;
  return_payload.UnpackTo(&reply);

  for (int i = 0; i < reply.warbles_size(); i++)
    r->warble_threads.push_back(reply.warbles(i));

  r->success = true;
  return;
}
void SetProfileReply(CommandResponse *r,
                     const google::protobuf::Any &return_payload) {
  ProfileReply reply;
  return_payload.UnpackTo(&reply);

  std::vector<std::string> followers;
  std::vector<std::string> following;
  for (int i = 0; i < reply.followers_size(); i++)
    followers.push_back(reply.followers(i));
  for (int i = 0; i < reply.following_size(); i++)
    following.push_back(reply.following(i));

  r->followers = followers;
  r->following = following;
  r->success = true;
  return;
}
// This takes in the struct payload and sets the requests
// Void function since success/failure stores in CommandResponse success flag
void FuncClient::Event(const int event_type, const Payload *p,
                       CommandResponse *r) {
  google::protobuf::Any *payload = new google::protobuf::Any;

  // Switch according to event_type and set Request with Payload data
  switch (event_type) {
    case kRegisterUserID: {
      SetRegisterUserRequest(p, payload);
      break;
    }
    case kWarbleID: {
      SetWarbleRequest(p, payload);
      break;
    }
    case kFollowUserID: {
      SetFollowRequest(p, payload);
      break;
    }
    case kReadID: {
      SetReadRequest(p, payload);
      break;
    }
    case kProfileID: {
      SetProfileRequest(p, payload);
      break;
    }
    default:
      std::cerr << "Invalid event type used in event function" << std::endl;
      return;
  }

  // Create EventRequest and set its payload, then pass to event function
  EventRequest event_request;
  event_request.set_event_type(event_type);
  event_request.set_allocated_payload(payload);

  // EventReply will hold data returned by func_server
  EventReply event_reply;

  // Context for the client.
  ClientContext context;

  Status status = stub_->event(&context, event_request, &event_reply);

  // Reply will now have payload if request was succesful
  google::protobuf::Any return_payload = event_reply.payload();
  // To-do look whether packFrom deallocates payload automatically

  // To-do - change this to glog
  if (!status.ok()) {
    VLOG(google::ERROR)
        << "Func service failed. The user or id may not exist. Please "
           "try a valid command.";
    return;
  }

  switch (event_type) {
    case kRegisterUserID: {
      SetRegisterUserReply(r, return_payload);
      break;
    }
    case kWarbleID: {
      SetWarbleReply(r, return_payload);
      break;
    }
    case kFollowUserID: {
      SetFollowUserReply(r, return_payload);
      break;
    }
    case kReadID: {
      SetReadReply(r, return_payload);
      break;
    }
    case kProfileID: {
      SetProfileReply(r, return_payload);
      break;
    }
    default:
      VLOG(google::ERROR) << "Invalid event_type used in event function.";
      return;
  }
}
}  // namespace dylanwarble
