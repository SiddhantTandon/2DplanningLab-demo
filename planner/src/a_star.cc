#include "a_star.h"

PriorityNode AStar::getCostforNode(Node expandedNode){
    float euclidean_cost = std::pow(std::pow((this->goal.row - expandedNode.row), 2)
                            + std::pow((this->goal.col - expandedNode.col), 2), 0.5);
    //float cost = this->move_cost + this->travel_cost + euclidean_cost;
    PriorityNode expandedNodePriority;
    expandedNodePriority.expandedNode = expandedNode;
    expandedNodePriority.cost = euclidean_cost;
    return expandedNodePriority;
}

void AStar::planTrajectory(){
    // put start node in expanded and visited
    PriorityNode startNode;
    startNode.expandedNode = this->start;
    startNode.cost = 0; // starting position
    this->expanded.push(startNode);
    this->visited.push_back(startNode);
    spdlog::info("Starting loop for expanded nodes!");
    // start the while loop
    while (!this->expanded.empty())
    {
        // pop the node
        PriorityNode popped_node = this->expanded.top();
        spdlog::info("Expanding node at ({},{})",popped_node.expandedNode.row, popped_node.expandedNode.col);
        spdlog::info("Its cost: {}", popped_node.cost);
        this->expanded.pop();
        // check if it is the goal -> break
        Node current_node = popped_node.expandedNode;
        this->travel_cost += this->move_cost;
        if (current_node == goal)
        {
            spdlog::info("Found goal!");
            this->visited.push_back(popped_node);
            break;
        }
        // get its neighbors
        std::vector<Node> neighbors = this->getNeighbors(current_node);
        // compute heuristics for each neighbor
        for (auto n: neighbors)
        {
            PriorityNode priority_n = this->getCostforNode(n);
            // add the neighbors to expanded queue
            if (std::find(visited.begin(), visited.end(), priority_n) != visited.end()) {
                spdlog::info("Skipping already visited!");
            } else {
                this->expanded.push(priority_n);
            }
        }
        // add the node to visited queue
        this->visited.push_back(popped_node);
    }
}

float AStar::getTravelCost(){
    return this->travel_cost;
}

std::vector<PriorityNode> AStar::getVisitedNodesList(){
    return this->visited;
}

void AStar::travelPath(){
    spdlog::info("Constructing path through backtracking.");
    if(goal.row == this->visited.back().expandedNode.row 
    && goal.col == this->visited.back().expandedNode.col){
        // characterize it
        Node parent = this->visited.back().parent;
        this->travel_path.push_front(this->goal);
        for (int i = (int)visited.size() - 1; i >= 0; --i){
            if (visited[i].parent._is_empty())
            {
                if (visited[i].expandedNode == this->start){
                    this->travel_path.push_front(visited[i].expandedNode);
                    spdlog::info("Path construction done!");
                    break;
                }
                else{
                    spdlog::error("Something is wrong, path construction has a node with empty parent that is not start!");
                }
            }
            if (visited[i].expandedNode == parent)
            {
                this->travel_path.push_front(visited[i].expandedNode);
                parent = visited[i].parent;
            }
        }
    }
    else{
        spdlog::error("Goal is not the same as visited path!");
    }
}

std::deque<Node> AStar::getTravelPath(){
    return this->travel_path;
}