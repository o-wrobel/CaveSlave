#include <iostream>
#include <SFML/Graphics.hpp>

#include "game.hpp"
#include "grid.hpp"
#include "tile.hpp"

Grid::Grid(int x, int y)
    : size(x, y)
    {
    
    tile_grid.resize(size.y, std::vector<Tile>(size.x, Tile()));

    Build_Grid();
}


void Grid::Build_Grid() {
    
    //build line of stone floor
    for (int i = 0; i < size.x; i++) {
        SetTile(i, 4,  "stone_floor");
    }

    //build stone underneath
    for (int row = 5; row < size.x; row++) {
        for (int col = 0; col < size.x; col++) {
            SetTile(col, row, "stone");
        }
    }

    //box of crates
    for (int col = 0; col < size.y; col++) {      
        SetTile(col, 0, "crate");      
    }
    for (int col = 0; col < size.x; col++) {      
        SetTile(col, size.y - 1, "crate");      
    }
    for (int row = 0; row < size.x; row++) {      
        SetTile(0, row, "crate");      
    }
    for (int row = 0; row < size.x; row++) {      
        SetTile(size.x - 1, row, "crate");      
    }
};


void Grid::SetTile(int x, int y, int tile_type) {
    if (x >= 0 && x < size.x && y >= 0 && y < size.x) {

        tile_grid[y][x].ChangeType(tile_type);

    } else {
        std::cerr << "Error: Attempted to set tile out of bounds (" << x << ", " << y << ")\n";
    }
}


void Grid::SetTile(int x, int y, const std::string& type_name) {
    if (x >= 0 && x < size.x && y >= 0 && y < size.x) {
        if (type_name != "") {
            tile_grid[y][x].ChangeType(GetTypeID(type_name));
        } else {
            std::cerr << "Error: No tile type specified (" << x << ", " << y << ")\n";
        }
        
    } else {
        std::cerr << "Error: Attempted to set tile out of bounds (" << x << ", " << y << ")\n";
    }
}


Tile Grid::GetTile(int x, int y) const {
    if (x >= 0 && x < size.x && y >= 0 && y < size.y) {
        return tile_grid[y][x];
    } else {
        std::cerr << "Error: Attempted to get tile out of bounds (" << x << ", " << y << ")\n";
        return Tile(0); // Return a default tile if out of bounds
    }
}


int Grid::GetTypeID(std::string type_name) {
    if (type_name == "stone") return 1;
    if (type_name == "stone_floor") return 2;
    if (type_name == "crate") return 3;
    return 0;
    
}
