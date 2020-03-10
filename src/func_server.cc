#include "func_server.h"
#include <grpcpp/grpcpp.h>
namespace dylanwarble
{

Status FuncServiceImpl::hook(ServerContext *context,
                             const HookRequest *hookrequest,
                             HookReply *hookreply)
{
  int event_type = hookrequest->event_type();
  const std::string event_function = hookrequest->event_function();

  if(event_type < 0 || event_type > event_table.size()) 
  {
    std::cout << "Error: attempting to hook invalid event_type" << std::endl;
    return Status::CANCELLED;
  }

  Event new_event(event_type, event_function, true);
  if(event_table.size() == event_type)
  {
    event_table.push_back(new_event);
    return Status::OK;
  }

  Event current_event = event_table[event_type];
  if(current_event.hooked == true)
  {
    std::cout << "Event ID is already being used" << std::endl;
    return Status::CANCELLED;
  }

  //Set the event at proper index to new event
  event_table[event_type] = new_event;
  return Status::OK;
}

Status FuncServiceImpl::unhook(ServerContext *context,
                               const UnhookRequest *unhookrequest,
                               UnhookReply *unhookreply)
{
  int event_type = unhookrequest->event_type();

  if(event_type < 0 || event_type > event_table.size()) 
  {
    std::cout << "Error: attempting to unhook invalid event_type" << std::endl;
    return Status::CANCELLED;
  }

  Event current_event = event_table[event_type];
  if(current_event.hooked == false)
  {
    std::cout << "Event_type is already unhooked" << std::endl;
    return Status::CANCELLED;
  }
  
  //Set event hooked status to false
  event_table[event_type].hooked = false;
  return Status::OK;
}

Status FuncServiceImpl::event(ServerContext *context,
                              const EventRequest *eventrequest,
                              EventReply *eventreply)
{

  const google::protobuf::Any payload = eventrequest->payload();

  WarbleFunctions func;

  switch (eventrequest->event_type())
  {
  case dylanwarble::FunctionID::kRegisterUserID:
  {
    RegisteruserRequest request;
    payload.UnpackTo(&request);
    std::string username = request.username();
    if (func.RegisterUser(username) == true)
      return Status::OK;
    else
      return Status::CANCELLED;
    break;
  }

    // case kWarbleID:
    //   SetWarbleRequest(p, payload);
    //   break;

  case dylanwarble::FunctionID::kFollowUserID:
  {
    FollowRequest request;
    payload.UnpackTo(&request);
    std::string username = request.username();
    std::string user_to_follow = request.to_follow();
    if (func.Follow(username, user_to_follow) == true) return Status::OK;
    else return Status::CANCELLED;
    break;
  }
    // case kReadID:
    //   SetReadRequest(p, payload);
    //   break;

  case kProfileID:
  {
    ProfileRequest request;
    payload.UnpackTo(&request);
    std::string username = request.username();
    func.Profile(username);
    break;
  }
  }
  // std::string s = request.username();
  // std::cerr << s << std::endl;

  return Status::OK;
}

void RunServer()
{
  std::string server_address("0.0.0.0:50000");
  FuncServiceImpl service;

  //Hook initial warble functions in setup of func infrastructure
  service.HookInitialWarbleFunctions();

  ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case, it corresponds to an *synchronous* service.
  builder.RegisterService(&service);

  // Finally assemble the server.
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Func server listening on " << server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

void FuncServiceImpl::HookInitialWarbleFunctions(){
  
  Event registeruser(kRegisterUserID, "Register user", true);
  Event warble(kWarbleID, "Warble", true);
  Event followuser(kFollowUserID, "Follow user", true);
  Event read(kReadID, "Read", true);
  Event profile(kProfileID, "Profile", true);

  event_table.push_back(registeruser);
  event_table.push_back(warble);
  event_table.push_back(followuser);
  event_table.push_back(read);
  event_table.push_back(profile);

  std::cout << "Setup: hooked the following Warble functions ... " << std::endl;

  for(int i = 0; i < 5; i++) std::cout << i << ") " << event_table[i].event_function << std::endl;
  
}

} // namespace dylanwarble

int main(int argc, char **argv)
{
  //Includes initial setup of warble functions
  dylanwarble::RunServer();
  
  return 0;
}
