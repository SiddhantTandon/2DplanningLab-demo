#include "planner_base.h"

Search::Search(Node start, Node goal){
    this->start = start;
    this->goal = goal;
}

bool Search::checkGoal(Node input){
    return (this->goal.row == input.row && this->goal.col == input.col); 
}

Node Search::getGoal(){
    return this->goal;
}

void Search::setGoal(Node newgoal){
    this->goal = newgoal;
}

void Search::setStart(Node newstart){
    this->start = newstart;
}

std::vector<Node> Search::getNeighbors(){
    // add something to return vectors
    std::vector<Node> neighbor_list;
    
    //TODO: add stuff here

    return neighbor_list;
}


Search::~Search(){
    spdlog::debug("Destroying base planner object!");
}