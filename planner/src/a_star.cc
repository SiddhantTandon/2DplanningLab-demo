#include "a_star.h"

PriorityNode AStar::getCostforNode(Node expandedNode){
    float euclidean_cost = std::pow(std::pow((this->goal.row - expandedNode.row), 2)
                            + std::pow((this->goal.col - expandedNode.col), 2), 0.5);
    float cost = this->move_cost + this->travel_cost + euclidean_cost;
    PriorityNode expandedNodePriority;
    expandedNodePriority.expandedNode = expandedNode;
    expandedNodePriority.cost = cost;
    return expandedNodePriority;
}

void AStar::planTrajectory(){
    // put start node in expanded and visited
    PriorityNode startNode;
    startNode.expandedNode = this->start;
    startNode.cost = 0; // starting position
    this->expanded.push(startNode);
    this->visited.push_back(this->start);
    // start the while loop
    while (!this->expanded.empty())
    {
        // pop the node
        PriorityNode popped_node = this->expanded.top();
        this->expanded.pop();
        // check if it is the goal -> break
        Node current_node = popped_node.expandedNode;
        this->travel_cost += this->move_cost;
        if (current_node.row == this->goal.row && current_node.col == this->goal.col)
        {
            spdlog::info("Found goal!");
            this->visited.push_back(current_node);
            break;
        }
        // get its neighbors
        std::vector<Node> neighbors = this->getNeighbors(current_node);
        // compute heuristics for each neighbor
        for (auto n: neighbors)
        {
            PriorityNode priority_n = this->getCostforNode(n);
            // add the neighbors to expanded queue
            this->expanded.push(priority_n);
        }
        // add the node to visited queue
        this->visited.push_back(current_node);
    }
}

float AStar::getTravelCost(){
    return this->travel_cost;
}

std::vector<Node> AStar::getVisitedNodesList(){
    return this->visited;
}