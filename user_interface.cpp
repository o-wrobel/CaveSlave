#include <SFML/Graphics.hpp>
#include "user_interface.hpp"
#include "game.hpp"


//USER INTERFACE


UserInterface::UserInterface(Game& game)
    : game_(game),
    window_(game_.window_),
    
    kTileResolution(8), //game_.kTileResolution doesn't work for some reason // NEEDS TO BE FIXED
    tile_preview_({window_.getSize().x - 60.f, 60.f}, game_.tile_textures_, kTileResolution, game_.kTileTypeCount),
    tile_overlay_(game_.tile_textures_, kTileResolution, game_.kTileTypeCount),
    fps_counter_(game_.font_),

    my_circle_(6)
{
    my_circle_.setFillColor(sf::Color::Blue);
    my_circle_.setOrigin({my_circle_.getRadius(), my_circle_.getRadius()});
    my_circle_.setPosition({(30) * 1.f, game_.window_.getSize().y - (30) * 1.f}); 
}


void UserInterface::Update() {
    tile_preview_.Update();
    tile_overlay_.Update(game_.mouse_grid_position_);
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


TilePreview::TilePreview(sf::Vector2f position, const std::vector<sf::Texture>& tile_textures, 
    int tile_resolution, int tile_type_count) 
    : kTileResolution(tile_resolution), 
    kTileTypeCount(tile_type_count), 
    kTileTextures(tile_textures),

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
}


void TilePreview::Update(){
    sprite_.setTexture(kTileTextures.at(tile_type_));
}


void TilePreview::Draw(sf::RenderWindow& window){
    window.draw(background_);
    window.draw(sprite_);
}

// TILE OVERLAY


TileOverlay::TileOverlay(const std::vector<sf::Texture>& tile_textures, int tile_resolution, int tile_type_count)
 :  kTileResolution(tile_resolution),
    kTileTypeCount(tile_type_count), 
    kTileTextures(tile_textures),
    sprite_rect_(sf::Vector2f(kTileResolution, kTileResolution)),
    tile_texture_(sf::Vector2u(kTileResolution, kTileResolution)),
    background_(sf::Vector2f(kTileResolution, kTileResolution)),
    sprite_(tile_texture_),
    tile_type_(1) //THIS SHIT HAS TO BE DECLARED IT DOES NOT DEFAULT TO ZERO
 {
    background_.setFillColor(sf::Color(0, 0, 0, 200));

    sprite_rect_.setFillColor(sf::Color::Transparent);
    sprite_rect_.setOutlineThickness(-1.f);
    sprite_rect_.setOutlineColor(sf::Color(255, 255, 255, 240));

 }


void TileOverlay::NextTileType(){
    tile_type_++;
    if (tile_type_ >= kTileTypeCount) {tile_type_ = 1;}
}


void TileOverlay::Update(const sf::Vector2i& grid_position){
    // grid_position_ = kGame.mouse_grid_position_;
    grid_position_ = grid_position;
    sprite_rect_.setPosition({grid_position_.x * kTileResolution * 1.f, grid_position_.y * kTileResolution * 1.f});
    sprite_.setPosition({grid_position_.x * kTileResolution * 1.f, grid_position_.y * kTileResolution * 1.f});
    background_.setPosition({grid_position_.x * kTileResolution * 1.f, grid_position_.y * kTileResolution * 1.f});
    sprite_.setTexture(kTileTextures.at(tile_type_));
    
}


void TileOverlay::Draw(sf::RenderWindow& window){
    window.draw(background_);
    window.draw(sprite_);
    window.draw(sprite_rect_);
}


// FPS COUNTER 


FPSCounter::FPSCounter(const sf::Font& font)
    :  frequency_(0.1f),
    frequency_2_(1.f),
    font_(font),
    text_(font_) ,
    text_size_(24)
{   
    timer_.restart();
    timer_2_.restart();
    text_.setCharacterSize(text_size_);
    text_.setFillColor(sf::Color::White);
    text_.setPosition({10.f, 10.f});
}

void FPSCounter::Update(const sf::Time& delta_time) {
    if (timer_2_.getElapsedTime().asSeconds() >= frequency_2_) {
        timer_2_.restart();
        average_framerate_ = (highest_framerate_ + lowest_framerate_) / 2;
        highest_framerate_ = framerate_;
        lowest_framerate_ = framerate_;
    }
    if (timer_.getElapsedTime().asSeconds() >= frequency_) {
        timer_.restart();
        framerate_ = static_cast<int>(1.f / delta_time.asSeconds());

        if (framerate_ > highest_framerate_) {
            highest_framerate_ = framerate_;
        }
        if (framerate_ < lowest_framerate_) {
            lowest_framerate_ = framerate_;
        }

        text_.setString(
            "FPS: " + std::to_string(framerate_) + '\n'
            // + "Highest: " + std::to_string(highest_framerate_) + '\n'
            // + "Lowest: " + std::to_string(lowest_framerate_) + '\n'
            + "Average: " + std::to_string(average_framerate_) + '\n'
        );
    }
}

void FPSCounter::Draw(sf::RenderWindow& window){
    window.draw(text_);
}