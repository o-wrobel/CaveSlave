#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <filesystem>
#include <SFML/Graphics.hpp>

class Game {
    private:
        const unsigned int WINDOW_SIZE_X = 720;
        const unsigned int WINDOW_SIZE_Y = 720;
        
        const int CENTER_Y = WINDOW_SIZE_Y / 2;
        const int CENTER_X = WINDOW_SIZE_X / 2;

        sf::RenderWindow window;
        sf::Texture my_texture;
        sf::Sprite my_sprite;
        sf::CircleShape my_circle;
    public:

        Game(int window_size_x, int window_size_y); // Constructor declaration
        void run();


};

#endif // GAME_HPP