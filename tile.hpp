#ifndef TILE_HPP
#define TILE_HPP

#include <iostream>
#include <SFML/Graphics.hpp>

class Tile {
    private:
        int type_id;

    public:
        Tile(int type);
        
        int GetType() const;
        void ChangeType(int new_type);

        bool type_changed;

};

#endif