#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "state_message.grpc.pb.h"
#include <chrono>
#include <thread>
#include <vector>
#include <spdlog/spdlog.h>

#include "common_utils.h"
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

        // service 
        ClientContext context;
        std::shared_ptr<ClientReaderWriter<AgentMessage, MapMessage>> stream(
            stub_->ShareState(&context));
        
        std::string filename = "../../maps/map_16x32_indoor.txt";
        MapGraph map(filename);
        if (map.setGrid())
        {
            Ego mock_agent("1", "ego");

            mock_agent.setMap(&map);

            mock_agent.setPosition(4, 1);
            Node goal{14, 28};
            mock_agent.addGoal(goal);
            Node goal2{1, 29};
            mock_agent.addGoal(goal2);
            mock_agent.showGoals();

            mock_agent.makePath();

            while (true){

                /*  THIS CAN BE THE AGENT LOOP FOR SENDING ALL AGENT MESSAGES  */
                // message to send
                if(mock_agent.remainingPathGreaterThanZero() && mock_agent.goalReached()){
                    mock_agent.updateForNextMessage();
                }
                else if(!mock_agent.remainingPathGreaterThanZero() && mock_agent.goalReached()){
                    spdlog::info("Agent has reached its goal!! ✅");
                    break;
                }
                else{
                    spdlog::info("Agent could not reach goal!! 😞");
                    break;
                }
                AgentMessage msg;

                // Set timestamp
                google::protobuf::Timestamp* ts = msg.mutable_timestamp();
                std::chrono::seconds sec = std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch());
                ts->set_seconds(sec.count() + 1);  //TODO: or use actual time

                // Add agent
                AgentState* agent = msg.add_agents();
                agent->set_agent_id(mock_agent.getID());

                // Set position
                Position* pos = agent->mutable_position();
                Node mock_agent_pos = mock_agent.getPosition(); // TODO: need to pop this 
                pos->set_row(mock_agent_pos.row);
                pos->set_col(mock_agent_pos.col);

                if (!stream->Write(msg)) {
                    std::cerr << "Write failed. Server may have closed the stream." << std::endl;
                    break;
                }

                /*  THEN END THE LOOP HERE AND WAIT FOR THE RESPONSE   */
                std::this_thread::sleep_for(std::chrono::seconds(1));

                MapMessage response; //TODO: make sure we are only giving back one response per timestamp
                if (!stream->Read(&response)) {
                    std::cout << "Server closed stream. Ending client." << std::endl;
                    break;
                }
                else{
                    std::cout << "Received MapMessage with timestamp: "
                            << response.timestamp().seconds() << std::endl;
                    std::cout << "Received position row: " << response.map_states().at(0).cell_state().position().row() << std::endl;
                    std::cout << "Received position col: " << response.map_states().at(0).cell_state().position().col() << std::endl;
                    std::cout << "Received value: " << response.map_states().at(0).cell_state().value() << std::endl;

                    //TODO: do something wit this message
                    // Ideally use this to figure out when we need to invoke the A* again

                }
            }
            stream->WritesDone();
            /* REQUIRED TO END CLIENT WHEN BIDRECTIONAL STREAM */
            MapMessage response;
            while(!stream->Read(&response)) {
                std::cout << "Server closed stream. Ending client." << std::endl;
                break;
            }

            /* NOW ACTUALLY ENDING CLIENT */
            Status status = stream->Finish();
            if (!status.ok()) {
                std::cerr << "ShareState rpc failed: " << status.error_message() << std::endl;
            }
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

/*
FIXME: Do this below instead of what is above ^

// TODO: For A* we need to ensure that the message back and forth can be used for 
// future versions of A* otherwise this will be too much of work

while (true) {

    // Send the message

    //TODO: pop the position and send it. 

    if (!stream->Write(request)) {
        std::cerr << "Write failed. Server may have closed the stream." << std::endl;
        break;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));  // Optional pacing

    // Wait for response
    if (!stream->Read(&response)) {
        std::cout << "Server closed stream. Ending client." << std::endl;
        break;
    }
    
    //TODO: For future A* versions - take the map input
    // adjust the process, and the position queue with new updates.
    // (ensure of committed path, choose the next node  from front of queue)
}

stream->WritesDone();

grpc::Status status = stream->Finish();
if (!status.ok()) {
    std::cerr << "RPC failed: " << status.error_message() << std::endl;
}

*/