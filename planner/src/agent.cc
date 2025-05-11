#include <iostream>
#include <chrono>
#include <string>
#include <thread>
#include <grpcpp/grpcpp.h>
#include "position.grpc.pb.h"
#include <random>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerWriter;
using grpc::Status;
using position::PositionService;
using position::PositionRequest;
using position::PositionResponse;
using position::Position;

class PositionServiceImpl final: public PositionService::Service{
    Status StreamPosition(ServerContext* context, const PositionRequest* request,
                          ServerWriter<PositionResponse>* writer) override {
        if (!request->enable()){ return Status::OK;}

        int duration = request->duration();
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dist_x(-10.0, 10.0);  // range for x
        std::uniform_real_distribution<> dist_y(-5.0, 5.0);    // range for y

        for (int i = 0; i < duration; i++){
            PositionResponse response;
            Position* pos =  response.mutable_position();
            pos->set_x(dist_x(gen));
            pos->set_y(dist_y(gen));

            writer->Write(response);
            std::this_thread::sleep_for(std::chrono::seconds(1));

        }
        return Status::OK;
            
        }

};

void RunServer(){
    std::string server_address("0.0.0.0:50051");
    PositionServiceImpl service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server Listening on " << server_address << std::endl;
    server->Wait();
}

int main (int argc, char**argv)
{
    RunServer();
    return 0;
}