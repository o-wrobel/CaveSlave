#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>

#include "grid.hpp"
#include "camera.hpp"

class Game {
    private:
        const sf::Vector2u kWindowSize; 
        const sf::Vector2u kWindowCenter;
        const int kTileResolution;
        static const int kTileTypeCount = 8;
        sf::RenderWindow window;
        sf::View view;
        Camera camera;

        sf::Clock clock;
        sf::Time delta_time;

        sf::Vector2i view_start_position;
        sf::Vector2i view_end_position;

        sf::Vector2i grid_size;
        Grid game_grid;

        sf::Texture tile_texture;
        sf::Texture tile_spritesheet;

        sf::Sprite tile_sprite;
        sf::FloatRect tile_preview_bounds;
        sf::Sprite tile_preview_sprite;

        sf::Vector2i mouse_position;
        sf::Vector2f mouse_world_position;
        sf::Vector2i mouse_grid_position;

        int tile_place_type;
        sf::Vector2i last_tile_placed_position;
        sf::CircleShape my_circle;

        // Input state variables

        bool input_up_held;
        bool input_down_held;
        bool input_left_held;
        bool input_right_held;

        bool spacebar_pressed;

        bool lmb_held;
        bool rmb_held;

        float mouse_wheel_delta;

        // Private methods

        void CheckEvents();
        void SetTextures();
        void DrawGrid(Grid grid);
        void DrawTile(Tile tile, int x, int y);
        void SetInputVariables();
        void HandleTilePlacing();
        void NextTileType();
        void SetTileSpriteTexture(sf::Sprite& sprite, int tile_type);
        void GetTextureFromSpritesheet(int index_x, int index_y, sf::Texture& spritesheet, int resolution, sf::Sprite& sprite);

    public:

        Game(unsigned int window_size_x, unsigned int window_size_y); // Constructor declaration
        void GameLoop();
        
};

#endif // GAME_HPP