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
    // put start node in pq and visited
    PriorityNode startNode;
    startNode.expandedNode = this->start;
    startNode.cost = 0; // starting position
    this->pq.push(startNode);
    this->visited.push_back(this->start);
    spdlog::info("Starting loop for expanded nodes!");
    // start the while loop
    while (!this->pq.empty())
    {
        // pop the node
        PriorityNode popped_node = this->pq.top();
        spdlog::info("Expanding node at ({},{})",popped_node.expandedNode.row, popped_node.expandedNode.col);
        spdlog::info("Its cost: {}", popped_node.cost);
        this->pq.pop();
        // check if it is the goal -> break
        Node current_node = popped_node.expandedNode;
        this->travel_cost += this->move_cost;
        if (current_node == goal)
        {
            spdlog::info("Found goal!");
            this->expanded.push_back(popped_node);
            this->reachedGoal = true;
            break;
        }
        // get its neighbors
        std::vector<Node> neighbors = this->getNeighbors(current_node);
        // compute heuristics for each neighbor
        for (auto n: neighbors)
        {
            PriorityNode priority_n = this->getCostforNode(n);
            spdlog::info("Node found for: ({},{})", priority_n.expandedNode.row, priority_n.expandedNode.col);
            priority_n.parent = current_node; // adding parent is important for extraction!
            // add the neighbors to priority queue if not previously visited
            if (std::find(visited.begin(), visited.end(), priority_n.expandedNode) != visited.end()) {
                spdlog::info("Skipping already visited!");
            } else {
                spdlog::info("Adding to priority list!");
                this->pq.push(priority_n);
                this->visited.push_back(priority_n.expandedNode);
            }
        }
        // add the node to visited queue
        this->expanded.push_back(popped_node);
        spdlog::info("Queue size: {}", this->pq.size());
    }
    if(this->pq.empty() && !this->reachedGoal){
        spdlog::info("No Goal Found!");
        this->reachedGoal = false;
    }

}

float AStar::getTravelCost(){
    return this->travel_cost;
}

std::vector<PriorityNode> AStar::getExpandedNodesList(){
    return this->expanded;
}

void AStar::travelPath(){
    spdlog::info("Constructing path through backtracking.");
    if(this->goal == this->expanded.back().expandedNode){
        // characterize it
        Node parent = this->expanded.back().parent;
        spdlog::info("Parent of back node: ({},{})", parent.row, parent.col);
        this->travel_path.push_front(this->goal);
        for (int i = (int)this->expanded.size() - 1; i >= 0; i--){
            if (this->expanded[i].parent._is_empty())
            {
                if (this->expanded[i].expandedNode == this->start){
                    this->travel_path.push_front(this->expanded[i].expandedNode);
                    spdlog::info("Path construction done!");
                    break;
                }
                else{
                    spdlog::error("Something is wrong, path construction has a node with empty parent that is not start!");
                }
            }
            if (this->expanded[i].expandedNode == parent)
            {
                this->travel_path.push_front(this->expanded[i].expandedNode);
                parent = this->expanded[i].parent;
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

bool AStar::reachableGoal(){
    return this->reachedGoal;
}