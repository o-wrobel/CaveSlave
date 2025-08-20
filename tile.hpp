#ifndef TILE_HPP
#define TILE_HPP

#include <iostream>
#include <SFML/Graphics.hpp>

class Tile {
    private:
    std::string tile_type;

    public:
        Tile(std::string type);
        std::string GetType(int position_x, int position_y) const;
        void ChangeType(std::string new_type);

            


};

#endif