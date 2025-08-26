#include <SFML/Graphics.hpp>
#include "user_interface.hpp"
#include "game.hpp"

UserInterface::UserInterface(Game& game)
    : game_(game),
    window_(game_.window_),
    
    kTileSpritesheet(game_.kTileSpritesheet),
    kTileResolution(game_.kTileResolution),
    kTileTypeCount(game_.kTileTypeCount),

    tile_texture_(sf::Vector2u(8, 8)),
    black_square_(sf::Vector2f(kTileResolution, kTileResolution)),
    tile_preview_sprite_(tile_texture_),
    tile_place_type_(1),

    my_circle_(6)
{
    my_circle_.setFillColor(sf::Color::Blue);
    my_circle_.setOrigin({my_circle_.getRadius(), my_circle_.getRadius()});
    my_circle_.setPosition({(30) * 1.f, (30) * 1.f});

    black_square_.setFillColor(sf::Color(0, 0, 0, 200));
    black_square_.setOrigin({kTileResolution/2.f, kTileResolution/2.f});
    black_square_.setPosition({window_.getSize().x * 0.92f, window_.getSize().y * 0.10f});
    black_square_.setScale(sf::Vector2f(6.f, 6.f));

    game_.SetTileSpriteTexture(tile_preview_sprite_, kTileSpritesheet, kTileResolution, tile_place_type_);
    tile_preview_sprite_.setOrigin({kTileResolution/2.f, kTileResolution/2.f});
    tile_preview_sprite_.setPosition({window_.getSize().x * 0.92f, window_.getSize().y * 0.10f});
    tile_preview_sprite_.setColor(sf::Color(255, 255, 255, 200));
    tile_preview_sprite_.setScale(sf::Vector2f(6.f, 6.f));
}

void UserInterface::NextTilePlaceType() {
    tile_place_type_++;
    if (tile_place_type_ >= kTileTypeCount) {
            tile_place_type_ = 1;
        }
    game_.SetTileSpriteTexture(tile_preview_sprite_, kTileSpritesheet, kTileResolution, tile_place_type_);
    return;
}

void UserInterface::Update() {
    my_circle_.setFillColor(sf::Color::Blue);
    // game_.SetTileSpriteTexture(tile_preview_sprite_, kTileSpritesheet, kTileResolution, tile_place_type_);
    return;
}

void UserInterface::Draw() {
    window_.draw(black_square_);
    window_.draw(tile_preview_sprite_);
    window_.draw(my_circle_);
    return;
}