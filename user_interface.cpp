#include <SFML/Graphics.hpp>
#include "user_interface.hpp"
#include "game.hpp"


//USER INTERFACE


UserInterface::UserInterface(Game& game)
    : game_(game),
    window_(game_.window_),
    
    kTileSpritesheet(game_.kTileSpritesheet),
    kTileResolution(8), //game_.kTileResolution doesn't work for some reason

    tile_preview_({window_.getSize().x - 60.f, 60.f}, kTileResolution, game_.kTileTypeCount),

    my_circle_(6)
{
    my_circle_.setFillColor(sf::Color::Blue);
    my_circle_.setOrigin({my_circle_.getRadius(), my_circle_.getRadius()});
    my_circle_.setPosition({(30) * 1.f, (30) * 1.f});

    game_.SetTileSpriteTexture(tile_preview_.GetSprite(), kTileSpritesheet, kTileResolution, tile_preview_.tile_type_);
}


void UserInterface::NextTilePlaceType() {
    tile_preview_.NextTileType();
    game_.SetTileSpriteTexture(tile_preview_.GetSprite(), kTileSpritesheet, kTileResolution, tile_preview_.tile_type_);
    return;
}


void UserInterface::Update() {
    my_circle_.setFillColor(sf::Color::Blue);
    // game_.SetTileSpriteTexture(tile_preview_sprite_, kTileSpritesheet, kTileResolution, tile_place_type_);
    return;
}


void UserInterface::Draw() {
    window_.draw(my_circle_);
    window_.draw(tile_preview_.GetBackground());
    window_.draw(tile_preview_.GetSprite());
    return;
}


// TILE PREVIEW


TilePreview::TilePreview(sf::Vector2f position, int tile_resolution, int tile_type_count) 
    : kTileResolution(tile_resolution), 
    kTileTypeCount(tile_type_count), 

    position_(position),

    tile_texture_(sf::Vector2u(tile_resolution, tile_resolution)),
    sprite_(tile_texture_),
    background_(sf::Vector2f(kTileResolution, kTileResolution)),

    tile_type_(1)

{

    background_.setFillColor(sf::Color(0, 0, 0, 200));
    background_.setOrigin({8/2.f, 8/2.f});
    background_.setPosition(position_);
    background_.setScale(sf::Vector2f(10.f, 10.f));

    sprite_.setOrigin({kTileResolution/2.f, kTileResolution/2.f});
    sprite_.setPosition(position_);
    sprite_.setColor(sf::Color(255, 255, 255, 200));
    sprite_.setScale(sf::Vector2f(10.f, 10.f));
}


void TilePreview::NextTileType(){
    tile_type_++;
    if (tile_type_ >= kTileTypeCount) {tile_type_ = 1;}
}


sf::Texture& TilePreview::GetTexture(){return tile_texture_;}


sf::Sprite& TilePreview::GetSprite() {return sprite_;}


sf::RectangleShape& TilePreview::GetBackground() {return background_;}
