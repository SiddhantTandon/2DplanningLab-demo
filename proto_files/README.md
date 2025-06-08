### How to create proto files using vcpkg

#### Python
```
python3 -m grpc_tools.protoc -I./proto --python_out=./platform/src --grpc_python_out=./platform/network state_message.proto
```

#### C++
```
/home/necromancer/vcpkg/packages/protobuf_x64-linux/tools/protobuf/protoc -I/home/necromancer/vcpkg/packages/protobuf_x64-linux/include/ --proto_path=./proto --cpp_out=./planner/src --grpc_out=./planner/network --plugin=protoc-gen-grpc=/home/necromancer/vcpkg/packages/grpc_x64-linux/tools/grpc/grpc_cpp_plugin state_message.proto
```