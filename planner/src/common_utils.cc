#include "common_utils.h"

MapGraph::MapGraph(std::string file){
    this->map_file = file;
}

void MapGraph::setGrid(){
    //read map file and set the grid
    // use the size to set the col and row values
}

Node MapGraph::getNode(int row, int col){
    Node tmp;
    tmp.row = row;
    tmp.col = col;
    tmp.map_value = this->grid[row * this->col_size + col];
    return tmp;
}

std::vector<Node> MapGraph::validEmptyNodes(){
    std::vector<Node> node_list;
    for (int i = 0; i < (this->row_size * this->col_size) - 1; i++)
    {
        if (this->grid[i] == "x"){
            Node tmp;
            tmp.row = i / this->col_size; // need to check this logic properly!
            tmp.col = i % this->col_size;
            spdlog::debug("Valid Location found at");
            tmp.map_value = "x";
            node_list.push_back(tmp);
        }
    }
    return node_list;
}

// use this only for previous cells of agents, beacause agents would be transmitting their positions anyway
void MapGraph::updateMap(int row, int col, std::string value){
    Node tmp;
    tmp = this->getNode(row, col);
    tmp.map_value = value; // doing this because we want to tell simulation what grid changed to what
    this->updatedCells.push(std::make_pair(tmp, value));
    this->grid[row * this->col_size + col] = value;
}

void MapGraph::emptyMapChangeRegister(){
    while (!this->updatedCells.empty())
    {
        this->updatedCells.pop();
    }
}

MapGraph::~MapGraph(){
    spdlog::debug("Destroying map object!");
}