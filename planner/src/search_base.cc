#include "search_base.h"

Search::Search(Node start, Node goal, MapGraph* map){
    this->start = start;
    this->goal = goal;
    this->setMapforSearch(map);
    this->printMapStats();
}

void Search::setMapforSearch(MapGraph* map){
    this->map = map;
}

bool Search::checkGoal(Node input){
    return (this->goal.row == input.row && this->goal.col == input.col); 
}

Node Search::showGoal(){
    return this->goal;
}

void Search::setGoal(Node newgoal){
    this->goal = newgoal;
}

void Search::setStart(Node newstart){
    this->start = newstart;
}

// Filtering out blocked cells since any search would do that 
std::vector<Node> Search::getNeighbors(Node current){
    // add something to return vectors
    spdlog::info("Searching neighbors...");
    std::vector<Node> neighbor_list;
    
    // left
    if (current.col -1 >= 0)
    {
        Node left = this->map->getNode(current.row, current.col - 1);
        if (left.map_value == "x"){
            neighbor_list.push_back(left);
        }
    }
    // right
    if (current.col +1 < this->map->getColSize())
    {
        Node right = this->map->getNode(current.row, current.col + 1);
        if (right.map_value == "x"){
            neighbor_list.push_back(right);
        }
    }
    // top
    if (current.row -1 >= 0)
    {
        Node top = this->map->getNode(current.row - 1, current.col);
        if (top.map_value == "x"){
            neighbor_list.push_back(top);
        }
    }
    // bottom
    if (current.row + 1 < this->map->getRowSize())
    {
        Node bottom = this->map->getNode(current.row + 1, current.col);
        if (bottom.map_value == "x"){
            neighbor_list.push_back(bottom);
        }
    }

    spdlog::info("Found neighbors!");
    return neighbor_list;
}

void Search::printMapStats(){
    this->map->printStats();
}

Search::~Search(){
    spdlog::debug("Destroying base planner object!");
}