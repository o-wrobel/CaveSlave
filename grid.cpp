#include <iostream>
#include <SFML/Graphics.hpp>

#include "game.hpp"
#include "grid.hpp"
#include "tile.hpp"

Grid::Grid(int x, int y)
    : size_x(x), size_y(y) {
    
    tile_grid.resize(size_y, std::vector<Tile>(size_x, Tile("null")));

    Build_Grid();
}

void Grid::Build_Grid() {
    //build line of stone
    for (int i = 0; i < size_x; i++) {
        SetTile(i, 4, "stone_floor");
    }

    //build stone underneath
    for (int row = 5; row < size_x; row++) {
        for (int col = 0; col < size_x; col++) {
            SetTile(col, row, "stone");
        }
    }
};

void Grid::SetTile(int x, int y, std::string tile_type) {
    if (x >= 0 && x < size_x && y >= 0 && y < size_y) {
        tile_grid[y][x].ChangeType(tile_type);
    } else {
        std::cerr << "Error: Attempted to set tile out of bounds (" << x << ", " << y << ")\n";
    }
}