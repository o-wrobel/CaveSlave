#ifndef USER_INTERFACE_HPP
#define USER_INTERFACE_HPP

#include <SFML/Graphics.hpp>
// #include "game.hpp"

class Game;


class FPSCounter{
private:
    sf::Clock timer_;
    const float frequency_;
    const sf::Font font_;
    sf::Text text_;
    const int text_size_;
    int framerate_;

public:
    FPSCounter(const sf::Font& font);
    void Update(const sf::Time& delta_time);
    void Draw(sf::RenderWindow& window);
};


class TilePreview{
private:
    const int kTileResolution;
    const int kTileTypeCount;

    sf::Vector2f position_;
    
    sf::Texture tile_texture_;
    
    

public:
    sf::Texture kTileSpritesheet;
    sf::Sprite sprite_;
    sf::RectangleShape background_;
    int tile_type_;
    TilePreview(sf::Vector2f position, const sf::Texture& spritesheet, int tile_resolution, int tile_type_count);
    void NextTileType();
    void Update();
    void Draw(sf::RenderWindow& window);
    sf::RectangleShape& GetBackground();

};


class TileOverlay{
private:
    const Game& kGame;
    const int kTileResolution_;
    sf::Vector2i grid_position_; 
    sf::RectangleShape sprite_;

public:
    TileOverlay(const Game& game, int tile_resolution);
    void Update();
    void Draw(sf::RenderWindow& window);
};


class UserInterface {
private:
    Game& game_;
    sf::RenderWindow& window_;
    int kTileResolution;

    sf::Texture placeholder;

public:

    TilePreview tile_preview_;
    TileOverlay tile_overlay_;
    sf::CircleShape my_circle_;
    FPSCounter fps_counter_;

    UserInterface();
    UserInterface(Game& game);
    void Update();
    void Draw();
};
#endif