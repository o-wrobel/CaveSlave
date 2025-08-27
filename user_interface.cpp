#include <SFML/Graphics.hpp>
#include "user_interface.hpp"
#include "game.hpp"


//USER INTERFACE


UserInterface::UserInterface(Game& game)
    : game_(game),
    window_(game_.window_),
    
    // kTileSpritesheet(game_.kTileSpritesheet),
    placeholder(sf::Vector2u(1, 1)),
    kTileResolution(8), //game_.kTileResolution doesn't work for some reason
    tile_preview_({window_.getSize().x - 60.f, 60.f}, placeholder, kTileResolution, game_.kTileTypeCount),
    tile_overlay_(game_, kTileResolution),
    fps_counter_(game_.font_),

    my_circle_(6)
{
    my_circle_.setFillColor(sf::Color::Blue);
    my_circle_.setOrigin({my_circle_.getRadius(), my_circle_.getRadius()});
    my_circle_.setPosition({(30) * 1.f, (50) * 1.f}); 
}


void UserInterface::Update() {
    my_circle_.setFillColor(sf::Color::Blue);
    tile_preview_.Update();
    fps_counter_.Update(game_.delta_time_);
    
    return;
}


void UserInterface::Draw() {
    window_.draw(my_circle_);
    tile_preview_.Draw(window_);
    // Not tile overlay because it's view dependent
    fps_counter_.Draw(window_);
    return;
}


// TILE PREVIEW


TilePreview::TilePreview(sf::Vector2f position, const sf::Texture& spritesheet, int tile_resolution, int tile_type_count) 
    : kTileResolution(tile_resolution), 
    kTileTypeCount(tile_type_count), 
    kTileSpritesheet(spritesheet),

    position_(position),

    tile_texture_(sf::Vector2u(kTileResolution, kTileResolution)),
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
    Game::SetSpriteTileTexture(sprite_, kTileSpritesheet, kTileResolution, tile_type_);
}


void TilePreview::Update(){
    Game::SetSpriteTileTexture(sprite_, kTileSpritesheet, kTileResolution, tile_type_);
}


void TilePreview::Draw(sf::RenderWindow& window){
    window.draw(background_);
    window.draw(sprite_);
}

// TILE OVERLAY


TileOverlay::TileOverlay(const Game& game, int tile_resolution)
 : kGame(game),
   kTileResolution_(tile_resolution),
   sprite_(sf::Vector2f(kTileResolution_, kTileResolution_))
 {
    sprite_.setFillColor(sf::Color::Transparent);
    sprite_.setOutlineThickness(-1.f);
    sprite_.setOutlineColor(sf::Color(255, 255, 255, 170));
 }


void TileOverlay::Update(){
    grid_position_ = kGame.mouse_grid_position_;
    sprite_.setPosition({grid_position_.x * kTileResolution_ * 1.f, grid_position_.y * kTileResolution_ * 1.f}); 
}


void TileOverlay::Draw(sf::RenderWindow& window){
    window.draw(sprite_);
}


// FPS COUNTER 


FPSCounter::FPSCounter(const sf::Font& font)
    :  frequency_(0.1f),
    font_(font),
    text_(font_) ,
    text_size_(24)
{   
    timer_.restart();
    text_.setCharacterSize(text_size_);
    text_.setFillColor(sf::Color::White);
    text_.setPosition({10.f, 10.f});
}

void FPSCounter::Update(const sf::Time& delta_time) {
    if (timer_.getElapsedTime().asSeconds() >= frequency_) {
        timer_.restart();
        framerate_ = static_cast<int>(1.f / delta_time.asSeconds());
        text_.setString(
            "FPS: " + std::to_string(framerate_) + '\n'
            + "Highest: "
        );
    }
}

void FPSCounter::Draw(sf::RenderWindow& window){
    window.draw(text_);
}