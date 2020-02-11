#include "func.grpc.pb.h"
#include "func_client.h"

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

void FuncClient::event(const int event_type, const Payload &p)
{
    // Data we are sending to the server.
    EventRequest request;
    request.set_event_type(event_type);
    request.set_allocated_payload(p);

    // Container for the data we expect from the server.
    EventReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->event(&context, request, &reply);

    //Reply will now have payload if request was succesful
}

int main(int argc, char **argv)
{
    FuncClient funcclient(grpc::CreateChannel(
        "localhost:50000", grpc::InsecureChannelCredentials()));

    return 0;
}
