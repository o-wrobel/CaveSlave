#ifndef GRID_HPP
#define GRID_HPP

#include <iostream>
#include <SFML/Graphics.hpp>

#include "tile.hpp"

class Grid {
    private:
    int size_x; int size_y;
    std::vector<std::vector<Tile>> tile_grid;

    void Init_Grid();

    public:
    Grid(int x, int y);

};

#endif