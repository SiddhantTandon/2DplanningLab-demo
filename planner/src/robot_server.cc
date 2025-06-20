#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "state_message.grpc.pb.h"
#include <chrono>
#include <thread>
#include <vector>
#include <spdlog/spdlog.h>

#include "agent.h"

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
        //random stuff for mock
        std::vector<int> choices{-1,0,1};
        std::random_device rd;  // seed source
        std::mt19937 gen(rd()); // random number generator
        std::uniform_int_distribution<> dist(0, choices.size() - 1);

        // service 
        ClientContext context;
        std::shared_ptr<ClientReaderWriter<AgentMessage, MapMessage>> stream(
            stub_->ShareState(&context));
        
        Ego mock_agent("1", "ego");
        
        // message to send
        AgentMessage msg;

        // Set timestamp
        google::protobuf::Timestamp* ts = msg.mutable_timestamp();
        std::chrono::seconds sec = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch());
        ts->set_seconds(sec.count() + 1);  // or use actual time

        // Add agent
        AgentState* agent = msg.add_agents();
        agent->set_agent_id(mock_agent.getID());

        // Set position
        Position* pos = agent->mutable_position();
        AgentPosition mock_agent_pos = mock_agent.getPosition();
        pos->set_row(mock_agent_pos.row);
        pos->set_col(mock_agent_pos.col);

        stream->Write(msg);

        MapMessage response;
        while (stream->Read(&response)) {
            std::cout << "Received MapMessage with timestamp: "
                      << response.timestamp().seconds() << std::endl;
            std::cout << "Received position row: " << response.map_states().at(0).cell_state().position().row() << std::endl;
            std::cout << "Received position col: " << response.map_states().at(0).cell_state().position().col() << std::endl;
            std::cout << "Received value: " << response.map_states().at(0).cell_state().value() << std::endl;

            int rand_row = choices[dist(gen)];
            int rand_col = choices[dist(gen)];
            AgentPosition new_pos{rand_row, rand_col};
            mock_agent.updatePosition(new_pos);

            AgentMessage msg;

            // Set timestamp
            google::protobuf::Timestamp* ts = msg.mutable_timestamp();
            std::chrono::seconds sec = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch());
            ts->set_seconds(sec.count() + 1);  // or use actual time

            // Add agent
            AgentState* agent = msg.add_agents();
            agent->set_agent_id(mock_agent.getID());

            // Set position
            Position* pos = agent->mutable_position();
            AgentPosition mock_agent_pos = mock_agent.getPosition();
            pos->set_row(mock_agent_pos.row);
            pos->set_col(mock_agent_pos.col);

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
