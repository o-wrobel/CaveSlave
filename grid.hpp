#ifndef GRID_HPP
#define GRID_HPP

#include <iostream>
#include <SFML/Graphics.hpp>

#include "tile.hpp"

class Grid {
    private:
    int size_x; int size_y;
    std::vector<std::vector<Tile>> tile_grid;

    void Build_Grid();

    public:
    Grid(int x, int y);
    void SetTile(int x, int y, std::string tile_type);
    Tile GetTile(int x, int y) const {
        if (x >= 0 && x < size_x && y >= 0 && y < size_y) {
            return tile_grid[y][x];
        } else {
            std::cerr << "Error: Attempted to get tile out of bounds (" << x << ", " << y << ")\n";
            return Tile("null"); // Return a default tile if out of bounds
        }
    }

};

#endif