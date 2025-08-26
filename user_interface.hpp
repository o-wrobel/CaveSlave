#ifndef USER_INTERFACE_HPP
#define USER_INTERFACE_HPP

#include <SFML/Graphics.hpp>

class Game;

class UserInterface {
    private:
        Game& game_;
        sf::RenderWindow& window_;
        const sf::Texture& kTileSpritesheet;
        int kTileResolution;

        const int kTileTypeCount;

        sf::Texture tile_texture_;
        sf::RectangleShape black_square_;
        
    public:
        sf::CircleShape my_circle_;
        sf::Sprite tile_preview_sprite_;
        int tile_place_type_;

        UserInterface();
        UserInterface(Game& game);
        void NextTilePlaceType();
        void Update();
        void Draw();
};

#endif