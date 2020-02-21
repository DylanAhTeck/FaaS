
#include "func_client.h"
#include <iostream>



//Allows service to specify that a function is valid
void FuncClient::hook(const int event_type, const std::string &event_function)
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
    if (status.ok())
        std::cout << "Hook SUCCESFUL" << std::endl;
}

//Service specifies that the function is invalid
void FuncClient::unhook(const int event_type)
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
        std::cout << "Unhook SUCCESFUL" << std::endl;
}

//This takes in the struct payload and sets the requests
void FuncClient::event(const int event_type, const Payload &p)
{

    google::protobuf::Any payload;
    //payload.PackFrom(p);

    switch (event_type)
    {
    case 0:
    {
        RegisteruserRequest request;
        request.set_username(p.username);
        payload.PackFrom(request);
        break;
    }
    case 1:
    {
        WarbleRequest request;
        request.set_username(p.username);
        request.set_text(p.text);
        request.set_parent_id(p.parent_id);
        payload.PackFrom(request);
        break;
    }

    case 2:
    {
        FollowRequest request;
        request.set_username(p.username);
        request.set_to_follow(p.to_follow);
        payload.PackFrom(request);
        break;
    }

    case 3:
    {
        ReadRequest request;
        request.set_warble_id(p.id);
        payload.PackFrom(request);
        break;
    }

    case 4:
    {
        ProfileRequest request;
        request.set_username(p.username);
        payload.PackFrom(request);
        break;
    }

    default:
        break;
    }

    //EventRequest.payload in this case would be one type of Warble request
    EventRequest request;
    request.set_event_type(event_type);
    request.set_allocated_payload(&payload);

    // Container for the data we expect from the server.
    EventReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    //EventReply.payload in this case would be a Warble Reply

    // The actual RPC.
    Status status = stub_->event(&context, request, &reply);

    //Reply will now have payload if request was succesful

    google::protobuf::Any return_payload = request.payload();

    if (!status.ok())
    {
        std::cout << "Error" << std::endl;
        return;
    }

    //If warble, return warble ID
    //If read, return warble thread
    //If profile, return followers and following

    //E.g for Warble
    if (event_type == 1)
    {
        WarbleReply *warble_reply;
        return_payload.UnpackTo(warble_reply);
        //Warble warble = warble_reply->warble();
        //std::cout << "Warble ID: " << warble.id << std::endl;
    }

    //Etc
}

int main(int argc, char **argv)
{
    FuncClient funcclient(grpc::CreateChannel(
        "localhost:50000", grpc::InsecureChannelCredentials()));

    return 0;
}
