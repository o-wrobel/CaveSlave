#ifndef USER_INTERFACE_HPP
#define USER_INTERFACE_HPP

#include <SFML/Graphics.hpp>

class Game;

class UserInterface {
    private:
        Game& game_;
        sf::Window& window_;

        sf::Texture tile_texture_;
        sf::RectangleShape black_square;
        
    public:
        sf::CircleShape my_circle_;
        sf::Sprite tile_preview_sprite_;

        UserInterface();
        UserInterface(Game& game);
        void Update();
        void Draw();
};

#endif