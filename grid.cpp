#include <iostream>
#include <SFML/Graphics.hpp>

#include "game.hpp"
#include "grid.hpp"
#include "tile.hpp"

Grid::Grid(int x, int y)
    : size_x(x), size_y(y) {
    
    tile_grid.resize(size_y, std::vector<Tile>(size_x, Tile("stone")));
}