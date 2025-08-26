#ifndef GRID_HPP
#define GRID_HPP

#include <iostream>
#include <unordered_map>
#include <vector>

#include "tile.hpp"

class Grid {
    private:
        int size_[2];

        std::vector<std::vector<Tile>> tile_grid_;

        void Build_Grid();
        static const std::unordered_map<std::string, int> kTileNames;

    public:
        Grid(int x, int y);
        void SetTile(int x, int y, int tile_type);
        void SetTile(int x, int y, const std::string& type_name);
        Tile GetTile(int x, int y) const;
        static int GetTypeID(std::string type_name);

};

#endif