#ifndef USER_INTERFACE_HPP
#define USER_INTERFACE_HPP

#include <SFML/Graphics.hpp>
// #include "game.hpp"

class Game;

class UserInterface {
private:
    Game& game_;
    sf::RenderWindow& window_;
    int kTileResolution;

    sf::Texture placeholder;

public:

    TilePreview tile_preview_;
    sf::CircleShape my_circle_;

    UserInterface();
    UserInterface(Game& game);
    void Update();
    void Draw();
};

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
    sf::Texture& GetTexture();
    sf::Sprite& GetSprite();
    sf::RectangleShape& GetBackground();

};

#endif