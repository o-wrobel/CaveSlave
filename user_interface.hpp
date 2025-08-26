#ifndef USER_INTERFACE_HPP
#define USER_INTERFACE_HPP

#include <SFML/Graphics.hpp>

class Game;

class TilePreview{
private:
    int kTileResolution;
    int kTileTypeCount;

    sf::Vector2f position_;
    
    sf::Texture tile_texture_;
    sf::Sprite sprite_;
    sf::RectangleShape background_;

public:
    int tile_type_;
    TilePreview(sf::Vector2f position, int tile_resolution, int tile_type_count);
    void NextTileType();
    sf::Texture& GetTexture();
    sf::Sprite& GetSprite();
    sf::RectangleShape& GetBackground();

};

class UserInterface {
private:
    Game& game_;
    sf::RenderWindow& window_;
    const sf::Texture& kTileSpritesheet;
    int kTileResolution;

public:

    TilePreview tile_preview_;
    sf::CircleShape my_circle_;

    UserInterface();
    UserInterface(Game& game);
    void NextTilePlaceType(); // The spritesheet texture getting function needs to be changed to move this
    void Update();
    void Draw();
};

#endif