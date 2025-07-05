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

void Ego::showGoals(){
    spdlog::info("Printing goals for Robot ..");
    for(auto g: this->all_goals){
        spdlog::info("Goal: ({},{})", g.row, g.col);
    }
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

    this->current_goal = all_goals[0];
    spdlog::info("Current Goal is: ({},{})", this->current_goal.row, this->current_goal.col);

    Node start = this->position;

    while (!this->all_goals_copy.empty())
    {
        Node goal = this->all_goals_copy.front();
        this->all_goals_copy.pop();
        spdlog::info("Start Pos: ({},{})", start.row, start.col);
        AStar a_star_planner(start, goal, this->map);
        spdlog::info("Starting A* to create a path for goal ({},{})", goal.row, goal.col);
        a_star_planner.planTrajectory();
        a_star_planner.travelPath();
        this->goalReachable = a_star_planner.reachableGoal();
        if(goalReachable){
            std::deque<Node> returned_list = a_star_planner.getTravelPath();
            while(!returned_list.empty())
            {
                spdlog::info("In the front is: ({},{})", returned_list.front().row, returned_list.front().col);
                this->position_list.push_back(returned_list.front());
                returned_list.pop_front();
            }
        }
        else{
            this->position_list.clear();
            break;
        }

        start = goal;
        spdlog::info("Start Pos: ({},{})", start.row, start.col);
    }
    
}

void Ego::updateForNextMessage(){
    this->position_list.erase(this->position_list.begin()); // remove the element at first
    this->position = this->position_list.front(); // the next element is now first
    auto goal_it = std::find(this->all_goals.begin(), this->all_goals.end(), this->position);
    if (goal_it != this->all_goals.end() && std::next(goal_it) != this->all_goals.end()) {
        if(std::next(goal_it) != this->all_goals.end()){
            this->current_goal = this->all_goals[std::distance(this->all_goals.begin(), std::next(goal_it))];
            spdlog::info("Current Goal is: ({},{})", this->current_goal.row, this->current_goal.col);
        }
    }
}

bool Ego::remainingPathGreaterThanZero(){
    return this->position_list.size() != 0;
}

bool Ego::goalReached(){
    return this->goalReachable;
}