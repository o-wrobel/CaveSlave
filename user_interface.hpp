#ifndef USER_INTERFACE_HPP
#define USER_INTERFACE_HPP

#include <SFML/Graphics.hpp>
#include <vector>

class Game;


class FPSCounter{
private:
    sf::Clock timer_;
    const float frequency_;

    sf::Clock timer_2_;
    const float frequency_2_;

    const sf::Font font_;
    sf::Text text_;
    const int text_size_;

    int framerate_;
    int highest_framerate_;
    int lowest_framerate_;
    int average_framerate_;

    // std::array<int, 10> framerate_records_;
    // int record_index_;

public:
    FPSCounter(const sf::Font& font);
    void Update(const sf::Time& delta_time);
    void Draw(sf::RenderWindow& window);
};


class TilePreview{
private:
    const int kTileResolution;
    const int kTileTypeCount;
    const std::vector<sf::Texture>& kTileTextures;

    sf::Vector2f position_;
    sf::RectangleShape background_;
    
    sf::Texture tile_texture_;
    
public:
    sf::Sprite sprite_; // causes bugs when private for some reason
    
    int tile_type_;
    TilePreview(sf::Vector2f position, const std::vector<sf::Texture>& tile_textures, int tile_resolution, int tile_type_count);
    void NextTileType();
    void Update();
    void Draw(sf::RenderWindow& window);

};


class TileOverlay{
private:
    const int kTileResolution;
    const int kTileTypeCount;
    const std::vector<sf::Texture>& kTileTextures;

    sf::Vector2i grid_position_; 
    sf::RectangleShape background_;
    sf::RectangleShape sprite_rect_;

    sf::Texture tile_texture_;
    sf::Sprite sprite_; 
public:
    
    
    int tile_type_;

    TileOverlay(const std::vector<sf::Texture>& tile_textures, int tile_resolution, int tile_type_count);
    void NextTileType();
    void Update(const sf::Vector2i& grid_position);
    void Draw(sf::RenderWindow& window);
};


class UserInterface {
private:
    Game& game_;
    sf::RenderWindow& window_;
    int kTileResolution;

public:

    TilePreview tile_preview_;
    TileOverlay tile_overlay_;
    sf::CircleShape my_circle_;
    FPSCounter fps_counter_;

    UserInterface(Game& game);
    void Update();
    void Draw();
};
#endif