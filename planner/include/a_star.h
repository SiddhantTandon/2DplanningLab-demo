#pragma once
#include "search_base.h"

/*
For Astar we will keep it simple
but what will happen in dynamic replanning
is that we will incorporate time
so technically our search space will be infite!
*/

struct PriorityNode{
    Node expandedNode;
    Node parent;
    float cost;
};

struct PriorityComparator{
    bool operator()(const PriorityNode&a, const PriorityNode& b) {
        // lower cost should be at the top of the queue
        return a.cost > b.cost;
    }
};

class AStar: public Search{
    protected:
        std::priority_queue<PriorityNode, std::vector<PriorityNode>, PriorityComparator> expanded;
        std::vector<Node> visited;
        const float move_cost = 1; // we are only moving one grid at a time even waiting has a cost
        float travel_cost = 0; // total cost till now for visited nodes
    public:
        AStar(Node start, Node goal, MapGraph* map): Search(start, goal, map){
            spdlog::info("Start Location: ({},{}) and Goal Location: ({},{})", start.row, start.col, goal.row, goal.col);
        };
        void planTrajectory(); // in TVA you can call this for each time?
        PriorityNode getCostforNode(Node expandedNode);
        float getTravelCost();
        std::vector<Node> getVisitedNodesList();
};