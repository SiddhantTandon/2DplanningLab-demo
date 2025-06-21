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

std::vector<AgentPosition> Ego::getGoals(){
    return this->sub_goals;
}

std::vector<BasicObject> Ego::getObjects(){
    return this->objects;
}

void Ego::updatePosition(AgentPosition update){
    this->position.row += update.row;
    this->position.col += update.col;
}