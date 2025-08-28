#include "game.hpp"
#include "grid.hpp"
#include "tile.hpp"
#include "camera.hpp"
#include "input_handler.hpp"

#include <SFML/Graphics.hpp>
#include <algorithm>

Game::Game(unsigned int window_size_x, unsigned int window_size_y, int framerate_limit)
  : kWindowSize({window_size_x, window_size_y}),
    kWindowCenter({kWindowSize.x / 2, kWindowSize.y / 2}),
    font_("assets/fonts/arial.ttf"),
    kTileSpritesheet("assets/sprites/tiles/tiles.png"),

    kTileResolution(8),
    
    window_(sf::VideoMode({kWindowSize.x, kWindowSize.y}), "project"),
    camera_(*this),
    user_interface_(*this),
    input_handler_(*this),

    clock_(),

    tile_texture_(sf::Vector2u(8, 8)),
    tile_textures_(kTileTypeCount),

    tile_sprite_(tile_texture_),

    grid_size_(64, 64),
    game_grid_(grid_size_.x, grid_size_.y),

    tile_place_type_(1)
        
{    
    if(framerate_limit) {window_.setFramerateLimit(framerate_limit);}
    // window_.setIcon();
    SetTileTextures();
}


void Game::GameLoop() {
    // run the program as long as the window is open
    while (window_.isOpen())
    {        
        delta_time_ = clock_.restart();
        user_interface_.my_circle_.setFillColor(sf::Color::Blue);
        // user_interface_.Update();
        
        CheckEvents(); // check all the window's events that were triggered since the last iteration of the loop
        input_handler_.CheckInput();

        UpdateMouseVariables();

        input_handler_.ExecuteInputsCamera();
        camera_.Update();

        input_handler_.ExecuteInputsGame(); 
        user_interface_.Update();
        // user_interface_.tile_preview_.Update();
        // user_interface_.tile_overlay_.Update();
        
        //view stuff
        view_ = camera_.GetView();
        window_.setView(view_);

        window_.clear(sf::Color::Black);

        //DRAWING

        //Draw game world objects
        DrawGrid(game_grid_);
        user_interface_.tile_overlay_.Draw(window_);

        window_.setView(window_.getDefaultView());

        // draw ui here...
        user_interface_.Draw();

        // end the current frame
        window_.display();

    }
}


void Game::CheckEvents() {
    while (const std::optional event = window_.pollEvent())
    {
        // "close requested" event: we close the window
        if (event->is<sf::Event::Closed>()){
            window_.close();
        }

        input_handler_.CheckEventInput(event);

    }
}


void Game::DrawGrid(Grid grid) {
    Tile tile;
    // only iterates over tiles within view bounds to improve performance
    // thanks windsurf
    grid_view_start_position_ = {
        static_cast<int>(camera_.view_start_position_.x / kTileResolution), 
        static_cast<int>(camera_.view_start_position_.y / kTileResolution)
    };
    grid_view_start_position_.x = std::clamp(grid_view_start_position_.x, 0, grid_size_.x - 1);
    grid_view_start_position_.y = std::clamp(grid_view_start_position_.y, 0, grid_size_.y - 1);

    grid_view_end_position_ = {
        static_cast<int>(camera_.view_end_position_.x / kTileResolution), 
        static_cast<int>(camera_.view_end_position_.y / kTileResolution)
    };
    grid_view_end_position_.x = std::clamp(grid_view_end_position_.x, 0, grid_size_.x - 1);
    grid_view_end_position_.y = std::clamp(grid_view_end_position_.y, 0, grid_size_.y - 1);
    for (int row = grid_view_start_position_.y; row < grid_view_end_position_.y + 1; row++) {
        for (int col = grid_view_start_position_.x; col < grid_view_end_position_.x + 1; col++) {
            tile = grid.GetTile(col, row);
            if (tile.GetType() != 0){DrawTile(tile, col, row);
}
        }
    }
}


void Game::DrawTile(Tile& tile, int x, int y) {
    if (tile.type_changed){
        tile_sprite_.setTexture(tile_textures_.at(tile.GetType()));
        // SetSpriteTileTexture(tile_sprite_, kTileSpritesheet, kTileResolution, tile.GetType());
    }
    tile.type_changed = false; 

    // set position for tile
    tile_sprite_.setPosition({x * kTileResolution * 1.f, y * kTileResolution * 1.f});

    // draw tile
    window_.draw(tile_sprite_);
}


void Game::PlaceTile(sf::Vector2i mouse_position, bool lmb_held) {
    switch (lmb_held) {
    case true:
        game_grid_.SetTile(mouse_grid_position_.x, mouse_grid_position_.y, tile_place_type_);
        last_tile_placed_position_ = mouse_grid_position_;
        break;
    case false:
        game_grid_.SetTile(mouse_grid_position_.x, mouse_grid_position_.y, "null");
        break;
    }
    
    last_tile_placed_position_ = mouse_grid_position_;
    return;
}


void Game::NextTilePlaceType() {
    tile_place_type_++;
    if (tile_place_type_ >= kTileTypeCount) {
            tile_place_type_ = 1;
        }
    user_interface_.tile_preview_.tile_type_ = tile_place_type_;
    user_interface_.tile_overlay_.tile_type_ = tile_place_type_;
    return;
}


void Game::UpdateMouseVariables() {
    mouse_world_position_ = window_.mapPixelToCoords(mouse_position_, camera_.GetView());
    mouse_grid_position_ = sf::Vector2i(
        mouse_world_position_.x / kTileResolution,
        mouse_world_position_.y / kTileResolution
    );
    mouse_grid_position_.x = std::clamp(mouse_grid_position_.x, 0, grid_size_.x - 1);
    mouse_grid_position_.y = std::clamp(mouse_grid_position_.y, 0, grid_size_.y - 1);
}


void Game::SetTileTextures() {
    for(int i = 0; i < kTileTypeCount; i++){
        tile_textures_[i] = GetSpriteTileTexture(kTileSpritesheet, kTileResolution, i);
    }
}


sf::Time Game::GetDeltaTime() {return delta_time_;}


sf::Texture Game::GetTextureFromSpritesheet(int index_x, int index_y, const sf::Texture& spritesheet, 
    int resolution) {

    sf::IntRect rect({index_x * resolution, index_y * resolution}, {resolution, resolution});
    sf::Texture subTexture(spritesheet.copyToImage(), false, rect);

    return subTexture;

}


sf::Texture Game::GetSpriteTileTexture(const sf::Texture& tile_spritesheet, int kTileResolution, int tile_type) {
    switch (tile_type)
    {
    case 1: //stone
        return GetTextureFromSpritesheet(0, 0, tile_spritesheet, kTileResolution);
        break;
    case 2: //stone_floor
        return GetTextureFromSpritesheet(1, 0, tile_spritesheet, kTileResolution);
        break;
    case 3: //gem
        return GetTextureFromSpritesheet(2, 0, tile_spritesheet, kTileResolution);
        break;
    case 4: //gold
        return GetTextureFromSpritesheet(3, 0, tile_spritesheet, kTileResolution);
        break;
    case 5: //trap
        return GetTextureFromSpritesheet(0, 1, tile_spritesheet, kTileResolution);
        break;
    case 6: //pebbles
        return GetTextureFromSpritesheet(2, 1, tile_spritesheet, kTileResolution);
        break;
    case 7: //crate
        return GetTextureFromSpritesheet(1, 1, tile_spritesheet, kTileResolution);
        break;
    
    default:
        return GetTextureFromSpritesheet(0, 0, tile_spritesheet, kTileResolution);;
        break;
    }
}



