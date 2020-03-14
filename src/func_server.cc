// Copyright 2020 Dylan Ah Teck

#include "func_server.h" //NOLINT

namespace dylanwarble
{

// Allows a function to be added to Func service
// Return Status::OK if successful, Status::CANCELLED if not
Status FuncServiceImpl::hook(ServerContext *context,
                             const HookRequest *hookrequest,
                             HookReply *hookreply)
{
  int event_type = hookrequest->event_type();
  const std::string event_function = hookrequest->event_function();

  if (event_type < 0 || event_type > event_table_.size())
  {
    VLOG(google::ERROR)
        << "HOOK REQUEST: attempting to hook invalid Event ID. Failed.";
    return Status::CANCELLED;
  }

  // Creates new Event
  Event new_event(event_type, event_function, true);

  // If new event_type, must be the next largest available event_type
  if (event_table_.size() == event_type)
  {
    VLOG(google::INFO) << "HOOK REQUEST: new event successfully hooked.";
    event_table_.push_back(new_event);
    return Status::OK;
  }

  Event current_event = event_table_[event_type];

  // Fail if Event ID is currently used by a hooked function
  if (current_event.hooked == true)
  {
    VLOG(google::ERROR)
        << "HOOK REQUEST: Event ID is already being used. Failed.";
    return Status::CANCELLED;
  }

  // If the current Event ID has been unhooked, set the event at proper index to
  // new event
  event_table_[event_type] = new_event;
  VLOG(google::INFO) << "HOOK REQUEST: new event successfully hooked.";
  return Status::OK;
}

// Removes a function from Func service
// Return Status::OK if successful, Status::CANCELLED if not
Status FuncServiceImpl::unhook(ServerContext *context,
                               const UnhookRequest *unhookrequest,
                               UnhookReply *unhookreply)
{
  int event_type = unhookrequest->event_type();

  // Fail if unhooking an invalid Event ID
  if (event_type < 0 || event_type >= event_table_.size())
  {
    VLOG(google::ERROR)
        << "UNHOOK REQUEST: attempting to unhook invalid Event ID. Failed.";
    return Status::CANCELLED;
  }

  // Fail if unhooking an Event that is already unhooked
  Event current_event = event_table_[event_type];
  if (current_event.hooked == false)
  {
    VLOG(google::WARNING)
        << "UNHOOK REQUEST: Event ID already unhooked. Failed.";
    return Status::CANCELLED;
  }

  // Set Event hooked status to false if valid request
  event_table_[event_type].hooked = false;
  VLOG(google::INFO) << "UNHOOK REQUEST: event successfully unhooked.";
  return Status::OK;
}

// Helper functions to process each Event ID

// Calls Warble's RegisterUser function and returns Status::OK
// if successful returns Status::OK, else returns Status::CANCELLED
Status ProcessRegisterUserRequest(const google::protobuf::Any payload)
{
  RegisteruserRequest request;
  WarbleFunctions warble_service;

  payload.UnpackTo(&request);
  std::string username = request.username();

  if (warble_service.RegisterUser(username) == true)
    return Status::OK;
  else
    return Status::CANCELLED;
}

// Unpacks payload to WarbleRequest and calls Warble's PostWarble function.
// Returns Status::OK if successful, else returns Status::CANCELLED
Status ProcessWarbleRequest(const google::protobuf::Any payload,
                            google::protobuf::Any *return_payload,
                            EventReply *eventreply)
{
  // Unpack message
  WarbleRequest request;
  WarbleFunctions warble_service;

  payload.UnpackTo(&request);

  // Retrieve data from request
  std::string username = request.username();
  std::string text = request.text();
  std::string reply_to_warble = request.parent_id();

  // Call warble function
  WarbleReply *warble_reply = new WarbleReply;
  bool success =
      warble_service.PostWarble(username, text, reply_to_warble, warble_reply);

  // Return status if warble function was unsuccessful
  if (!success)
    return Status::CANCELLED;

  // Package and send reply
  return_payload->PackFrom(*warble_reply);
  delete warble_reply;
  eventreply->set_allocated_payload(return_payload);
  return Status::OK;
}

// Calls Warble's Follow function and returns Status::OK
// if successful returns Status::OK, else returns Status::CANCELLED
Status ProcessFollowRequest(const google::protobuf::Any payload)
{
  FollowRequest request;
  WarbleFunctions warble_service;
  payload.UnpackTo(&request);
  std::string username = request.username();
  std::string user_to_follow = request.to_follow();
  if (warble_service.Follow(username, user_to_follow) == true)
    return Status::OK;
  else
    return Status::CANCELLED;
}

// Unpacks payload to ReadRequest and retrieves warble threads using Warble's
// Read function Adds repeated Warbles as payload to EventReply Returns
// Status::OK if successful, else returns Status::CANCELLED
Status ProcessReadRequest(const google::protobuf::Any payload,
                          google::protobuf::Any *return_payload,
                          EventReply *eventreply)
{
  ReadRequest request;
  Warble warble;
  WarbleFunctions warble_service;
  payload.UnpackTo(&request);

  std::vector<Warble *> *warble_thread = new std::vector<Warble *>;
  std::string id = request.warble_id();

  // Warble's Read Function stores the warble threads in vector pointer
  if (warble_service.Read(id, warble_thread) != true)
  {
    return Status::CANCELLED;
  }

  ReadReply reply;

  // Add Warble to ReadRequest's repeated Warble Message field
  for (int i = 0; i < warble_thread->size(); i++)
  {
    Warble *warble = reply.add_warbles();
    warble->set_id(warble_thread->at(i)->id());
    warble->set_text(warble_thread->at(i)->text());
    warble->set_parent_id(warble_thread->at(i)->parent_id());
    warble->set_username(warble_thread->at(i)->username());

    // Add timestamp to warble as well
    Timestamp *timestamp_ptr = new Timestamp;
    timestamp_ptr->set_seconds(warble_thread->at(i)->timestamp().seconds());
    timestamp_ptr->set_useconds(warble_thread->at(i)->timestamp().useconds());
    warble->set_allocated_timestamp(timestamp_ptr);

    delete warble_thread->at(i);
  }

  return_payload->PackFrom(reply);
  eventreply->set_allocated_payload(return_payload);
  return Status::OK;
}

// Unpacks payload to ProfileRequest and retrieves followers/following lists
// using Warble's Profile function Adds follows and following as payload to
// EventReply Returns Status::OK if successful, else returns Status::CANCELLED
Status ProcessProfileRequest(const google::protobuf::Any payload,
                             google::protobuf::Any *return_payload,
                             EventReply *eventreply)
{
  ProfileRequest request;
  WarbleFunctions warble_service;
  payload.UnpackTo(&request);

  std::string username = request.username();
  std::vector<std::string> *followers = new std::vector<std::string>;
  std::vector<std::string> *following = new std::vector<std::string>;

  // Retrieve followers and following using Warble's Profile function
  if (warble_service.Profile(username, followers, following) == false)
    return Status::CANCELLED;

  // Add followers and following vector to ProfileRequest's message fields
  ProfileReply reply;
  for (int i = 0; i < followers->size(); i++)
  {
    reply.add_followers(followers->at(i));
  }
  for (int i = 0; i < following->size(); i++)
  {
    reply.add_following(following->at(i));
  }

  return_payload->PackFrom(reply);
  delete followers;
  delete following;
  eventreply->set_allocated_payload(return_payload);
  return Status::OK;
}

// Receives events of a particular type with a payload, processes request
// Return Status::OK if successful, Status::CANCELLED if not
Status FuncServiceImpl::event(ServerContext *context,
                              const EventRequest *eventrequest,
                              EventReply *eventreply)
{
  const google::protobuf::Any payload = eventrequest->payload();
  int eventid = eventrequest->event_type();

  // Return Status::CANCELLED if Event ID is not a hooked function
  if (eventid < 0 || eventid >= event_table_.size())
  {
    VLOG(google::ERROR) << "EVENT REQUEST: Event ID is invalid. Failed.";
  }

  // return_payload is deallocated in set_allocated_payload
  google::protobuf::Any *return_payload = new google::protobuf::Any;
  WarbleFunctions warble_service;

  // Call Processing function depending on Event ID
  switch (eventid)
  {
  case kRegisterUserID:
  {
    delete return_payload;
    return ProcessRegisterUserRequest(payload);
  }

  case kWarbleID:
  {
    return ProcessWarbleRequest(payload, return_payload, eventreply);
  }

  case kFollowUserID:
  {
    delete return_payload;
    return ProcessFollowRequest(payload);
  }

  case kReadID:
  {
    return ProcessReadRequest(payload, return_payload, eventreply);
  }

  case kProfileID:
  {
    return ProcessProfileRequest(payload, return_payload, eventreply);
  }

  default:
    return Status::CANCELLED;
  }
}

// Sets up the Func Server and hooks the initial Warble functions
void RunServerAndHookWarbleFunctions()
{
  std::string server_address("0.0.0.0:50000");
  FuncServiceImpl service;

  // Hook initial warble functions in setup of func infrastructure
  service.HookInitialWarbleFunctions();

  ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients.
  builder.RegisterService(&service);

  // Finally assemble the server.
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Func server listening on " << server_address << std::endl;

  // Wait for the server to shutdown.
  server->Wait();
}

// Registers the allowed warble functions
void FuncServiceImpl::HookInitialWarbleFunctions()
{
  // Creates the Event structs
  Event registeruser(kRegisterUserID, kRegisterUserFunctionName, true);
  Event warble(kWarbleID, kWarbleFunctionName, true);
  Event followuser(kFollowUserID, kFollowFunctionName, true);
  Event read(kReadID, kReadFunctionName, true);
  Event profile(kProfileID, kProfileFunctionName, true);

  // Adds the Events to the registered list of events
  event_table_.push_back(registeruser);
  event_table_.push_back(warble);
  event_table_.push_back(followuser);
  event_table_.push_back(read);
  event_table_.push_back(profile);

  std::cout << "Setup: hooked the following Warble functions ... " << std::endl;

  // Use cout not VLOG since this always needs to be shown
  for (int i = 0; i < 5; i++)
  {
    std::cout << i << ") " << event_table_[i].event_function << std::endl;
  }
}

} // namespace dylanwarble

int main(int argc, char **argv)
{
  // Initialize Google's logging library.
  google::InitGoogleLogging(argv[0]);
  FLAGS_logtostderr = 1;

  // Includes initial setup of warble functions
  dylanwarble::RunServerAndHookWarbleFunctions();

  return 0;
}
