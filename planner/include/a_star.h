#pragma once
#include "search_base.h"

/*
For Astar we will keep it simple
but what will happen in dynamic replanning
is that we will incorporate time
so technically our search space will be infite!
*/
class AStar: public Search{
    protected:
        std::priority_queue<Node> expanded;
        std::vector<Node> visited;
    public:
        AStar(Node start, Node goal): Search(start, goal){
            spdlog::info("Start Location: {} and Goal Location: {}", start, goal);
        };
        std::vector<Node> planTrajectory(); // in TVA you can call this for each time?

};