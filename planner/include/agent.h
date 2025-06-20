#include <iostream>
#include <vector>
#include <string>

struct Position{
    int row;
    int col;
};

class BasicObject{
    protected:
        int id;
        Position position;
        std::string agent_type;
    public:
        BasicObject(std::string id, std::string agent_type);
        ~BasicObject();
        Position getPosition();
        std::string getType();
        int getID();
};

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
        std::vector<Position> sub_goals;
        Position current_goal;
        std::vector<BasicObject> objects;
    public:
        Ego();
        ~Ego();
        std::vector<Position> getGoals();
        std::vector<BasicObject> getObjects();
};