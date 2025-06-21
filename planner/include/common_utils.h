#pragma once
#include <string>
#include <vector>
#include <queue>
#include <iostream>
#include <tuple>
#include <spdlog/spdlog.h>

struct Node{
    int row;
    int col;
    std::string map_value; // this will also be used for "type" of agents/objects
};

class MapGraph{
    private:
        std::string map_file;
        std::vector<int> grid; // we will use single line vector since that is most efficient for real time
    public:
        MapGraph(std::string file);
        void setGrid();
        Node getNode(int row, int col);
        std::vector<Node> validEmptyNodes(); // for random start locations
        void updateMap(int row, int col);
};