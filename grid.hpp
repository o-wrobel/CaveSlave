#ifndef GRID_HPP
#define GRID_HPP

#include <iostream>
#include <SFML/Graphics.hpp>

#include "tile.hpp"

class Grid {
    private:
    sf::Vector2i size;
    std::vector<std::vector<Tile>> tile_grid;

    void Build_Grid();

    public:
    Grid(int x, int y);
    void SetTile(int x, int y, std::string tile_type);
    Tile GetTile(int x, int y) const;

};

#endif