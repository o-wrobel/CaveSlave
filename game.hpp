#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>

#include "grid.hpp"
#include "camera.hpp"
#include "input_handler.hpp"
#include "user_interface.hpp"

// class UserInterface;

class Game {
    private:
        const sf::Vector2u kWindowSize; 
        const sf::Vector2u kWindowCenter;

        sf::View view_;

        sf::Clock clock_;
        

        sf::Vector2i grid_size_;
        Grid game_grid_;
        sf::Vector2i grid_view_start_position_;
        sf::Vector2i grid_view_end_position_;

        sf::Texture tile_texture_;

        sf::Sprite tile_sprite_;

        sf::Vector2i last_tile_placed_position_;

        // Private methods

        void CheckEvents();
        void DrawGrid(Grid grid);
        void DrawTile(Tile& tile, int x, int y);
        void UpdateMouseVariables();
        
        static void SetTextureFromSpritesheet(int index_x, int index_y, const sf::Texture& spritesheet, int resolution, sf::Sprite& sprite);

    public:

        sf::RenderWindow window_;
        sf::Font font_;
        Camera camera_;
        UserInterface user_interface_;
        InputHandler input_handler_;
        
        sf::Time delta_time_;
        const sf::Texture kTileSpritesheet;

        sf::Vector2i mouse_position_;
        sf::Vector2f mouse_world_position_;
        sf::Vector2i mouse_grid_position_;

        const int kTileResolution;
        static const int kTileTypeCount = 8;
        int tile_place_type_;

        Game(unsigned int window_size_x, unsigned int window_size_y, int framerate_limit = 0); // Constructor declaration
        void GameLoop();

        void PlaceTile(sf::Vector2i mouse_position, bool mouse_button_held);
        void NextTilePlaceType();
        static void SetSpriteTileTexture(sf::Sprite& sprite, const sf::Texture& tile_spritesheet, int resolution, int tile_type);
        sf::Time GetDeltaTime();
        
};

#endif // GAME_HPP