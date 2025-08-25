#ifndef TILE_HPP
#define TILE_HPP

#include <iostream>

class Tile {
    private:
        int type_id_;

    public:
        Tile(int type = 0);
        
        int GetType() const;
        void ChangeType(int new_type);

        bool type_changed;

};

#endif