#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <SFML/Graphics.hpp>

#include "grid.hpp"

class Game {
    private:
        const unsigned int WINDOW_SIZE_X;
        const unsigned int WINDOW_SIZE_Y;
        
        const int CENTER_Y;
        const int CENTER_X;

        float zoom_scale;

        int grid_size_x, grid_size_y;

        Grid game_grid;

        sf::RenderWindow window;
        sf::Texture stone_texture;
        sf::Texture stone_floor_texture;
        sf::Texture crate_texture;
        sf::Texture tile_texture;
        sf::Sprite tile_sprite;
        sf::CircleShape my_circle;

        sf::Clock clock;
        sf::Time delta_time;

        sf::Vector2i mouse_position;

        sf::Vector2f camera_position;

        bool up_pressed;
        bool down_pressed;
        bool left_pressed;
        bool right_pressed;

        bool forward_pressed;
        bool backward_pressed;
        

    public:

        Game(int window_size_x, int window_size_y); // Constructor declaration
        void GameLoop();
        void DrawGrid();

        void HandleInput();


};

#endif // GAME_HPP