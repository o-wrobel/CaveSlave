#include <iostream>
#include <SFML/Graphics.hpp>

#include "tile.hpp"

Tile::Tile(int type) : tile_id(type) {
    // Constructor implementation can be added if needed
}


int Tile::GetType() const {
    return tile_id;
}


void Tile::ChangeType(int new_type) {
    tile_id = new_type;
}
