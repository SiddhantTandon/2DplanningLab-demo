# 2DplanningLab-demo
This repository is for showcasing skills in motion planning to recruiters and hiring managers

## How to run?
1. terminal 1 -  go to build folder under planner 
2. run the following command `./robot_server`
3. terminal 2 - go to platform directory
4. run the command `python3 -m src.run_platform` 

### Other important notes
1. When making the proto generated files, and putting them in different folders, we had to make network a module, in doing so, we need to adjust the relative import for `position_pb2_grpc.py`