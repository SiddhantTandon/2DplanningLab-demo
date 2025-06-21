#pragma once
#include "common_utils.h"

struct AgentPosition{
    int row;
    int col;
};

class BasicObject{
    protected:
        std::string id;
        AgentPosition position;
        std::string agent_type;
        MapGraph* map; // should use pointer here since we need to update the same map object
    public:
        BasicObject(std::string id, std::string agent_type);
        ~BasicObject();
        void setPosition(int row, int col);
        AgentPosition getPosition();
        std::string getType();
        std::string getID();
        void updateMapCurrentPosition(int row, int col);
};

//TODO: adding definitions for dynamic later
class DynamicObstacle: public BasicObject{
    private:
        std::string agent_type;
    public:
        DynamicObstacle();
        ~DynamicObstacle();
        std::string get_obstacleType();
};

class Ego: public BasicObject{
    private:
        std::vector<AgentPosition> sub_goals;
        AgentPosition current_goal;
        std::vector<BasicObject> objects;
    public:
        Ego(std::string id, std::string agent_type) : BasicObject(id, agent_type){
            spdlog::info("Loaded agent successfully!");
        };
        ~Ego(){spdlog::debug("Destroyed Ego instance");};
        std::vector<AgentPosition> getGoals();
        std::vector<BasicObject> getObjects();
        void updatePosition(AgentPosition update);
};