#include "common_utils.h"

MapGraph::MapGraph(std::string file){
    this->map_file = file;
}


void MapGraph::extractMapDimensions(){
    std::string file = std::filesystem::path(this->map_file).filename().string();
    std::regex pattern(R"(map_(\d+)x(\d+)_.*)");

    std::smatch match;
    if (std::regex_match(file, match, pattern)) {
        this->row_size = std::stoi(match[1]); 
        this->col_size = std::stoi(match[2]);  
    }
    else{
        spdlog::error("Could not extract the dimensions from filename {}", this->map_file);
    }
}

int MapGraph::getColSize(){
    return this->col_size;
}

int MapGraph::getRowSize(){
    return this->row_size;
}

bool MapGraph::setGrid(){
    //read map file and set the grid
    this->extractMapDimensions();
    std::ifstream infile(this->map_file);
    if (!infile.is_open()) {
        std::cerr << "Failed to open file: " << this->map_file << std::endl;
        return false;
    }
    std::string line;
    int count = 0;
    while (std::getline(infile, line) && count < this->row_size) {
        if (line.size() != this->col_size){
            spdlog::error("Inconsistent row width detected!");
            return false;
        }

        for (char c : line) {
            grid.push_back(std::string(1, c));
        }
        count++;
    }

    infile.close();
    return true;
}

Node MapGraph::getNode(int row, int col){
    Node tmp;
    tmp.row = row;
    tmp.col = col;
    tmp.map_value = this->grid[row * this->col_size + col];
    return tmp;
}

std::vector<Node> MapGraph::validEmptyNodes(){
    std::vector<Node> node_list;
    for (int i = 0; i < (this->row_size * this->col_size) - 1; i++)
    {
        if (this->grid[i] == "x"){
            Node tmp;
            tmp.row = i / this->col_size; // need to check this logic properly!
            tmp.col = i % this->col_size;
            spdlog::debug("Valid Location found at");
            tmp.map_value = "x";
            node_list.push_back(tmp);
        }
    }
    return node_list;
}

// use this only for previous cells of agents, beacause agents would be transmitting their positions anyway
void MapGraph::updateMap(int row, int col, std::string value){
    Node tmp;
    tmp = this->getNode(row, col);
    tmp.map_value = value; // doing this because we want to tell simulation what grid changed to what
    this->updatedCells.push(std::make_pair(tmp, value));
    this->grid[row * this->col_size + col] = value;
}

void MapGraph::emptyMapChangeRegister(){
    while (!this->updatedCells.empty())
    {
        this->updatedCells.pop();
    }
}


void MapGraph::printStats(){
    spdlog::info("This is the size of the col: ", this->col_size);
    spdlog::info("This is the size of the row: {}", this->row_size);
}

MapGraph::~MapGraph(){
    spdlog::debug("Destroying map object!");
}