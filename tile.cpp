#include "tile.hpp"

Tile::Tile(int type) : type_id_(type) , type_changed(true) {
    // Constructor implementation can be added if needed
}


int Tile::GetType() const {
    return type_id_;
}


void Tile::ChangeType(int new_type) {
    type_id_ = new_type;
    type_changed = true;
}
