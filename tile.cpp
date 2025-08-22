#include <iostream>
#include <SFML/Graphics.hpp>

#include "tile.hpp"

Tile::Tile(int type) : type_id(type) , type_changed(true) {
    // Constructor implementation can be added if needed
}


int Tile::GetType() const {
    return type_id;
}


void Tile::ChangeType(int new_type) {
    type_id = new_type;
    type_changed = true;
}
