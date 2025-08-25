#include <iostream>

#include "game.hpp"
#include "grid.hpp"
#include "tile.hpp"

Grid::Grid(int x, int y)
    : size_(x, y)
    {

    tile_grid_.resize(size_.y, std::vector<Tile>(size_.x, Tile()));

    Build_Grid();
}


void Grid::Build_Grid() {
    
    //build line of stone floor
    for (int i = 0; i < size_.x; i++) {
        SetTile(i, 4,  "stone_floor");
    }

    //build stone underneath
    for (int row = 5; row < size_.x; row++) {
        for (int col = 0; col < size_.x; col++) {
            SetTile(col, row, "stone");
        }
    }

    //box of crates
    for (int col = 0; col < size_.y; col++) {      
        SetTile(col, 0, "crate");      
    }
    for (int col = 0; col < size_.x; col++) {      
        SetTile(col, size_.y - 1, "crate");      
    }
    for (int row = 0; row < size_.x; row++) {      
        SetTile(0, row, "crate");      
    }
    for (int row = 0; row < size_.x; row++) {      
        SetTile(size_.x - 1, row, "crate");      
    }
};


void Grid::SetTile(int x, int y, int tile_type) {
    if (x >= 0 && x < size_.x && y >= 0 && y < size_.x) {

        tile_grid_[y][x].ChangeType(tile_type);

    } else {
        std::cerr << "Error: Attempted to set tile out of bounds (" << x << ", " << y << ")\n";
    }
}


void Grid::SetTile(int x, int y, const std::string& type_name) {
    if (x >= 0 && x < size_.x && y >= 0 && y < size_.x) {
        if (type_name != "") {
            tile_grid_[y][x].ChangeType(GetTypeID(type_name));
        } else {
            std::cerr << "Error: No tile type specified (" << x << ", " << y << ")\n";
        }
        
    } else {
        std::cerr << "Error: Attempted to set tile out of bounds (" << x << ", " << y << ")\n";
    }
}


Tile Grid::GetTile(int x, int y) const {
    if (x >= 0 && x < size_.x && y >= 0 && y < size_.y) {
        return tile_grid_[y][x];
    } else {
        std::cerr << "Error: Attempted to get tile out of bounds (" << x << ", " << y << ")\n";
        return Tile(0); // Return a default tile if out of bounds
    }
}


int Grid::GetTypeID(std::string type_name) {
    return kTileNames.at(type_name);
}

const std::unordered_map<std::string, int> Grid::kTileNames = {
    {"null", 0},
    {"stone", 1},
    {"stone_floor", 2},
    {"gem", 3},
    {"gold", 4},
    {"trap", 5},
    {"pebbles", 6},
    {"crate", 7}
};

