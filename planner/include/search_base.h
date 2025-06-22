#pragma once
#include "common_utils.h"

// This will be basic search
class Search{
    protected:
        Node start;
        Node goal;
        MapGraph* map;
    public:
        Search(Node start, Node goal, MapGraph* map);
        ~Search();
        void setMapforSearch(MapGraph* map);
        std::vector<Node> getNeighbors(Node current);
        Node showGoal();
        bool checkGoal(Node input);
        void setGoal(Node newgoal);
        void setStart(Node newstart);
        void printMapStats();
};
