// #include <iostream>
// #include <chrono>
// #include <string>
// #include <thread>
// #include <grpcpp/grpcpp.h>
// #include "position.grpc.pb.h"
// #include <random>

// using grpc::Server;
// using grpc::ServerBuilder;
// using grpc::ServerContext;
// using grpc::ServerWriter;
// using grpc::Status;
// using position::PositionService;
// using position::PositionRequest;
// using position::PositionResponse;
// using position::Position;

// class PositionServiceImpl final: public PositionService::Service{
//     Status StreamPosition(ServerContext* context, const PositionRequest* request,
//                           ServerWriter<PositionResponse>* writer) override {
//         if (!request->enable()){ return Status::OK;}
        
//         // dummy position settings
//         int duration = request->duration();
//         std::random_device rd;
//         std::mt19937 gen(rd());
//         std::uniform_real_distribution<> dist_x(-64.0, 64.0);  // range for x
//         std::uniform_real_distribution<> dist_y(-64.0, 64.0);    // range for y

//         for (int i = 0; i < duration; i++){
//             PositionResponse response;
//             Position* pos =  response.mutable_position();
//             pos->set_x(dist_x(gen));
//             pos->set_y(dist_y(gen));

//             writer->Write(response);
//             std::this_thread::sleep_for(std::chrono::seconds(1));

//         }
//         return Status::OK;
            
//         }

// };

// void RunServer(){
//     std::string server_address("0.0.0.0:50051");
//     PositionServiceImpl service;

//     ServerBuilder builder;
//     builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
//     builder.RegisterService(&service);

//     std::unique_ptr<Server> server(builder.BuildAndStart());
//     std::cout << "Server Listening on " << server_address << std::endl;
//     server->Wait();
// }

// int main (int argc, char**argv)
// {
//     RunServer();
//     return 0;
// }

#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "state_message.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReaderWriter;
using grpc::Status;
using state_message::AgentMessage;
using state_message::MapMessage;
using state_message::ShareStateService;
using state_message::AgentState;
using state_message::Position;

class ShareStateClient {
public:
    ShareStateClient(std::shared_ptr<Channel> channel)
        : stub_(ShareStateService::NewStub(channel)) {}

    void ShareState() {
        ClientContext context;
        std::shared_ptr<ClientReaderWriter<AgentMessage, MapMessage>> stream(
            stub_->ShareState(&context));

        AgentMessage msg;

        // Set timestamp
        google::protobuf::Timestamp* ts = msg.mutable_timestamp();
        std::chrono::seconds sec = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch());
        ts->set_seconds(sec.count() + 1);  // or use actual time

        // Add agent
        AgentState* agent = msg.add_agents();
        agent->set_agent_id("agent1");

        // Set position
        Position* pos = agent->mutable_position();
        pos->set_row(1);
        pos->set_col(2);

        stream->Write(msg);
        stream->WritesDone();

        MapMessage response;
        while (stream->Read(&response)) {
            std::cout << "Received MapMessage with timestamp: "
                      << response.timestamp().seconds() << std::endl;
        }

        Status status = stream->Finish();
        if (!status.ok()) {
            std::cerr << "ShareState rpc failed: " << status.error_message() << std::endl;
        }
    }

private:
    std::unique_ptr<ShareStateService::Stub> stub_;
};

int main() {
    ShareStateClient client(grpc::CreateChannel(
        "localhost:50051", grpc::InsecureChannelCredentials()));
    client.ShareState();
    return 0;
}
