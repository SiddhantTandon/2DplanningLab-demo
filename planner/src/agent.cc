#include "agent.h"

BasicObject::BasicObject(std::string id, std::string agent_type){
    this->id = id;
    this->agent_type = agent_type;
}

BasicObject::~BasicObject(){
    std::cout<<"Deleting BasicObject instance" << std::endl;
}

void BasicObject::setPosition(int row, int col){
    this->position.row = row;
    this->position.col = col;
}

Node BasicObject::getPosition(){
    spdlog::info("Ego position is at: {},{}", this->position.row, this->position.col);
    return this->position;
}

std::string BasicObject::getType(){
    return this->agent_type;
}

std::string BasicObject::getID(){
    return this->id;
}

void BasicObject::updateMapCurrentPosition(int row, int col){
    std::string empty_space = "x"; // this is because all objects behind them should leave empty space at the current timestep
    this->map->updateMap(row, col, empty_space);
}

void BasicObject::setMap(MapGraph* map){
    this->map = map;
}

std::vector<Node> BasicObject::getPath(){
    return this->position_list;
}

std::vector<Node> Ego::showGoals(){
    return this->all_goals;
}

std::vector<BasicObject> Ego::getObjects(){
    return this->objects;
}

void Ego::addGoal(Node sub_goal){
    this->all_goals.push_back(sub_goal);
}

void Ego::updatePosition(Node update){
    //TODO: delete this afterwards
    spdlog::info("Delete this to get proper updates!");
    int delta_row = update.row - this->position.row; 
    int delta_col = update.col - this->position.col;
    this->updateMapCurrentPosition(update.row, update.col);
    this->position.row += delta_row;
    this->position.col += delta_col;    
    //this->position.row += update.row;
    //this->position.col += update.col;
}

void Ego::saveToMovementTrace(std::string move){
    this->movementTrace.push_back(move);
}

void Ego::makePath(){

    // fill queue
    for (int i =0; i < this->all_goals.size(); i++)
    {
        this->all_goals_copy.push(this->all_goals[i]);
    }

    Node start = this->position;

    while (!this->all_goals_copy.empty())
    {
        Node goal = this->all_goals_copy.front();
        this->all_goals_copy.pop();
        AStar a_star_planner(start, goal, this->map);
        spdlog::info("Starting A* to create a path");
        a_star_planner.planTrajectory();
        std::vector<Node> returned_list = a_star_planner.getVisitedNodesList();
        for (int j = 0; j < returned_list.size(); j++)
        {
            this->position_list.push_back(returned_list[j]);
        }

        Node start = goal;
    }
    
}

