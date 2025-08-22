#ifndef TILE_HPP
#define TILE_HPP

#include <iostream>
#include <SFML/Graphics.hpp>

class Tile {
    private:
    int tile_id;

    public:
        Tile(int type);
        int GetType() const;
        void ChangeType(int new_type);

};

#endif