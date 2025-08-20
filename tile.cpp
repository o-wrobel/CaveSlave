#include <iostream>
#include <SFML/Graphics.hpp>

#include "tile.hpp"

Tile::Tile(std::string type) : tile_type(type) {
    // Constructor implementation can be added if needed
}


std::string Tile::GetType() const {
    return tile_type;
}


void Tile::ChangeType(std::string new_type) {
    tile_type = new_type;
}