#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <SFML/Graphics.hpp>

#include "grid.hpp"

class Game {
    private:
        const sf::Vector2u kWindowSize; 
        const sf::Vector2u kWindowCenter;
        const int kTileResolution;
        const int kTileTypeCount = 4;
        sf::RenderWindow window;
        sf::View view;
        sf::Clock clock;
        sf::Time delta_time;

        sf::FloatRect view_bounds;
        sf::Vector2f view_center;
        sf::Vector2f view_size;
        float view_zoom_factor;

        sf::Vector2i view_start_position;
        sf::Vector2i view_end_position;

        sf::Vector2i grid_size;
        Grid game_grid;
        
        sf::Texture stone_texture;
        sf::Texture stone_floor_texture;
        sf::Texture crate_texture;
        sf::Texture tile_texture;
        sf::Sprite tile_sprite;

        sf::Vector2i mouse_position;
        sf::Vector2f mouse_world_position;
        sf::Vector2i mouse_grid_position;

        int tile_place_type;
        sf::CircleShape my_circle;

        // Input state variables

        bool up_pressed;
        bool down_pressed;
        bool left_pressed;
        bool right_pressed;

        bool forward_pressed;
        bool backward_pressed;

        bool lmb_clicked;
        bool rmb_clicked;

        // Private methods

        void DrawGrid(Grid grid);
        void DrawTile(Grid grid, int x, int y);
        void HandleInput();
        void HandleCamera();
        void HandleTilePlacing();
        void SetViewVariables();

    public:

        Game(unsigned int window_size_x, unsigned int window_size_y); // Constructor declaration
        void GameLoop();
        
};

#endif // GAME_HPP