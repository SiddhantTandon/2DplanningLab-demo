#pragma once
#include <string>
#include <vector>
#include <queue>
#include <deque>
#include <algorithm>
#include <iostream>
#include <set>
#include <cmath>
#include <fstream>
#include <regex>
#include <filesystem>
#include <spdlog/spdlog.h>

struct Node{
    int row = -1;
    int col = -1;
    std::string map_value; // this will also be used for "type" of agents/objects
    bool operator==(const Node& other) const {
        return row == other.row && col == other.col;
    }
    // this is to help with path loop
    bool _is_empty(){
        return row == -1 && col == -1;
    }
};

class MapGraph{
    private:
        std::string map_file;
        std::vector<std::string> grid; // we will use single line vector since that is most efficient for real time
        int row_size = 0;
        int col_size = 0;
        std::queue<std::pair<Node, std::string>> updatedCells; //  just use queue 
    public:
        MapGraph(std::string file);
        ~MapGraph();
        void extractMapDimensions();
        int getColSize();
        int getRowSize();
        bool setGrid();
        Node getNode(int row, int col);
        std::vector<Node> validEmptyNodes(); // for random start locations
        void updateMap(int row, int col, std::string value);
        void emptyMapChangeRegister();
        std::vector<Node> getMapChanges();
        void printStats();
};