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

AgentPosition BasicObject::getPosition(){
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

std::vector<AgentPosition> Ego::getGoals(){
    return this->sub_goals;
}

std::vector<BasicObject> Ego::getObjects(){
    return this->objects;
}

void Ego::updatePosition(AgentPosition update){
    this->updateMapCurrentPosition(update.row, update.col);
    this->position.row += update.row;
    this->position.col += update.col;
}