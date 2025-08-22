#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <SFML/Graphics.hpp>

#include "grid.hpp"

class Game {
    private:
        const sf::Vector2u kWindowSize; 
        const sf::Vector2u kWindowCenter;
        sf::RenderWindow window;
        sf::View view;
        sf::Clock clock;
        sf::Time delta_time;

        
        const int tile_resoultion;
        sf::Vector2i grid_size;

        Grid game_grid;

        
        sf::Texture stone_texture;
        sf::Texture stone_floor_texture;
        sf::Texture crate_texture;
        sf::Texture tile_texture;
        sf::Sprite tile_sprite;

        sf::CircleShape my_circle;

        sf::Vector2i mouse_position;

        // Input state variables

        bool up_pressed;
        bool down_pressed;
        bool left_pressed;
        bool right_pressed;

        bool forward_pressed;
        bool backward_pressed;

        // Private methods

        void DrawGrid();
        void DrawTile(int x, int y);
        void HandleInput();
        void HandleCamera();

    public:

        Game(unsigned int window_size_x, unsigned int window_size_y); // Constructor declaration
        void GameLoop();
        
};

#endif // GAME_HPP