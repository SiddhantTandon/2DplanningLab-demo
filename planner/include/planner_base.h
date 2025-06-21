#pragma once
#include "common_utils.h"

// This will be basic search
class Search{
    protected:
        // something
        Node start;
        Node goal;
    public:
        Search(Node start, Node goal);
        ~Search();
        void getNeighbors();
        void getGoal();
        bool checkGoal(Node input);
        void setGoal();
        void setStart();
};
