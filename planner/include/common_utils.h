#pragma once
#include <string>
#include <vector>
#include <queue>
#include <iostream>
#include <set>
#include <spdlog/spdlog.h>

struct Node{
    int row;
    int col;
    std::string map_value; // this will also be used for "type" of agents/objects
};

class MapGraph{
    private:
        std::string map_file;
        std::vector<std::string> grid; // we will use single line vector since that is most efficient for real time
        int row_size;
        int col_size;
        std::queue<std::pair<Node, std::string>> updatedCells; //  just use queue 
    public:
        MapGraph(std::string file);
        ~MapGraph();
        int getColSize();
        int getRowSize();
        void setGrid();
        Node getNode(int row, int col);
        std::vector<Node> validEmptyNodes(); // for random start locations
        void updateMap(int row, int col, std::string value);
        void emptyMapChangeRegister();
        std::vector<Node> getMapChanges();
};