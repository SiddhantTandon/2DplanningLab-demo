#pragma once
#include "common_utils.h"
#include "a_star.h"

//TODO: maybe need to add a time struct later for tracking time

class BasicObject{
    protected:
        std::string id;
        Node position;
        std::string agent_type;
        MapGraph* map; // should use pointer here since we need to update the same map object
        std::vector<Node> position_list; //TODO: confusion between queue or vector
    public:
        BasicObject(std::string id, std::string agent_type);
        ~BasicObject();
        void setPosition(int row, int col);
        Node getPosition();
        std::string getType();
        std::string getID();
        void updateMapCurrentPosition(int row, int col);
};

//TODO: adding definitions for dynamic later
class DynamicObstacle: public BasicObject{
    private:
        std::string behavior;
    public:
        DynamicObstacle();
        ~DynamicObstacle();
        std::string get_obstacleType();
};

class Ego: public BasicObject{
    private:
        std::vector<Node> all_goals;
        std::queue<Node> all_goals_copy;
        Node current_goal; // this is for debugging
        std::vector<BasicObject> objects;
        Node previous_position;
        std::vector<std::string> movementTrace; //useful for moving and xAi approaches?
    public:
        Ego(std::string id, std::string agent_type) : BasicObject(id, agent_type){
            spdlog::info("Loaded agent successfully!");
        };
        ~Ego(){spdlog::debug("Destroyed Ego instance");};
        std::vector<Node> showGoals();
        void addGoal(Node sub_goal);
        std::vector<BasicObject> getObjects();
        void updatePosition(Node update);
        void saveToMovementTrace(std::string move);
        void getPath();
};