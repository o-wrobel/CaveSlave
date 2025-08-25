#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>

#include "grid.hpp"
#include "camera.hpp"
#include "input_handler.hpp"

class Game {
    private:
        const sf::Vector2u kWindowSize; 
        const sf::Vector2u kWindowCenter;
        const int kTileResolution;
        static const int kTileTypeCount = 8;
        sf::RenderWindow window_;
        sf::View view_;
        Camera camera_;
        InputHandler input_handler_;

        sf::Clock clock_;
        sf::Time delta_time_;

        sf::Vector2i grid_size_;
        Grid game_grid_;
        sf::Vector2i grid_view_start_position_;
        sf::Vector2i grid_view_end_position_;

        sf::Texture tile_texture_;
        sf::Texture tile_spritesheet_;

        sf::Sprite tile_sprite_;
        sf::FloatRect tile_preview_bounds_;
        sf::Sprite tile_preview_sprite_;

        sf::Vector2f mouse_world_position_;
        sf::Vector2i mouse_grid_position_;

        int tile_place_type_;
        sf::Vector2i last_tile_placed_position_;
        sf::CircleShape my_circle_;
        

        // Private methods

        void CheckEvents();
        void SetTextures();
        void DrawGrid(Grid grid);
        void DrawTile(Tile& tile, int x, int y);
        void SetInputVariables();
        void PlaceTile(sf::Vector2i mouse_position, bool mouse_button_held);
        void NextTileType();
        void SetTileSpriteTexture(sf::Sprite& sprite, int tile_type);
        void GetTextureFromSpritesheet(int index_x, int index_y, sf::Texture& spritesheet, int resolution, sf::Sprite& sprite);

    public:

        Game(unsigned int window_size_x, unsigned int window_size_y); // Constructor declaration
        void GameLoop();
        
        friend class InputHandler;


        
};

#endif // GAME_HPP