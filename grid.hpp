#ifndef GRID_HPP
#define GRID_HPP

#include <iostream>
#include <unordered_map>
#include <SFML/Graphics.hpp>

#include "tile.hpp"

class Grid {
    private:
        sf::Vector2i size;
        std::vector<std::vector<Tile>> tile_grid;

        void Build_Grid();
        static const std::unordered_map<std::string, int> tile_names;

    public:
        Grid(int x, int y);
        void SetTile(int x, int y, int tile_type);
        void SetTile(int x, int y, const std::string& type_name);
        Tile GetTile(int x, int y) const;
        static int GetTypeID(std::string type_name);

};

#endif