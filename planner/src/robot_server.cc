#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "state_message.grpc.pb.h"
#include <chrono>
#include <thread>

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

        MapMessage response;
        while (stream->Read(&response)) {
            std::cout << "Received MapMessage with timestamp: "
                      << response.timestamp().seconds() << std::endl;
            std::cout << "Received position row: " << response.map_states().at(0).cell_state().position().row() << std::endl;
            std::cout << "Received position col: " << response.map_states().at(0).cell_state().position().col() << std::endl;
            std::cout << "Received value: " << response.map_states().at(0).cell_state().value() << std::endl;
            stream->Write(msg);
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        stream->WritesDone();
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
