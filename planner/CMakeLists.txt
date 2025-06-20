cmake_minimum_required(VERSION 3.15)
project(PositionStreamServer)

find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_executable(robot_server
    src/robot_server.cc
    network/state_message.pb.cc
    network/state_message.grpc.pb.cc
    src/agent.cc
)
# cmake .. -DCMAKE_TOOLCHAIN_FILE=/home/necromancer/vcpkg/scripts/buildsystems/vcpkg.cmake
target_include_directories(robot_server PRIVATE 
    ${Protobuf_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/network
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(robot_server
    gRPC::grpc++
    protobuf::libprotobuf
    spdlog::spdlog
)