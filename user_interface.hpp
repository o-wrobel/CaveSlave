#ifndef USER_INTERFACE_HPP
#define USER_INTERFACE_HPP

#include <SFML/Graphics.hpp>
// #include "game.hpp"

class Game;

class TilePreview{
private:
    const int kTileResolution;
    const int kTileTypeCount;

    sf::Vector2f position_;
    
    sf::Texture tile_texture_;
    sf::Sprite sprite_;
    sf::RectangleShape background_;

public:
    sf::Texture kTileSpritesheet;
    int tile_type_;
    TilePreview(sf::Vector2f position, const sf::Texture& spritesheet, int tile_resolution, int tile_type_count);
    void NextTileType();
    void Update();
    sf::Texture& GetTexture();
    sf::Sprite& GetSprite();
    sf::RectangleShape& GetBackground();

};


class TileOverlay{
private:
    Game& game_;
    const int kTileResolution_;
    sf::Vector2i grid_position_; 
    sf::RectangleShape sprite_;

public:
    TileOverlay(Game& game, int tile_resolution);
    void Update();
    void Draw();
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

    UserInterface();
    UserInterface(Game& game);
    void Update();
    void Draw();
};
#endif