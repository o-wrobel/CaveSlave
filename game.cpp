#include "game.hpp"
#include "grid.hpp"
#include "tile.hpp"
#include "camera.hpp"
#include "input_handler.hpp"

#include <SFML/Graphics.hpp>

Game::Game(unsigned int window_size_x, unsigned int window_size_y, int framerate_limit)
  : kWindowSize({window_size_x, window_size_y}),
    kWindowCenter({kWindowSize.x / 2, kWindowSize.y / 2}),

    kTileResolution(8),
    
    window_(sf::VideoMode({kWindowSize.x, kWindowSize.y}), "project"),
    camera_(*this),
    user_interface_(*this),
    input_handler_(*this),

    clock_(),

    tile_texture_(sf::Vector2u(8, 8)),
    kTileSpritesheet("sprites/tiles/tiles.png"),

    tile_sprite_(tile_texture_),
    tile_preview_sprite_(tile_texture_),
    black_square_(sf::Vector2f(kTileResolution, kTileResolution)),

    grid_size_(64, 64),
    game_grid_(grid_size_.x, grid_size_.y),

    tile_place_type_(1)
        
{    
    window_.setFramerateLimit(framerate_limit);

    // set position for tile
    SetTileSpriteTexture(tile_preview_sprite_, kTileSpritesheet, kTileResolution, tile_place_type_);
    tile_preview_sprite_.setOrigin({kTileResolution/2.f, kTileResolution/2.f});
    tile_preview_sprite_.setPosition({kWindowSize.x * 0.92f, kWindowSize.y * 0.10f});
    tile_preview_sprite_.setColor(sf::Color(255, 255, 255, 200));
    tile_preview_sprite_.setScale(sf::Vector2f(6.f, 6.f));


    black_square_.setFillColor(sf::Color::White); //sf::Color(0, 0, 0, 200)
    black_square_.setOrigin({kTileResolution/2.f, kTileResolution/2.f});
    black_square_.setPosition({kWindowSize.x * 0.5f, kWindowSize.y * 0.5f}); //{kWindowSize.x * 0.92f, kWindowSize.y * 0.10f}
    black_square_.setScale(sf::Vector2f(6.f, 6.f));


}


void Game::GameLoop() {
    // run the program as long as the window is open
    while (window_.isOpen())
    {        
        delta_time_ = clock_.restart();
        user_interface_.Update();

        CheckEvents(); // check all the window's events that were triggered since the last iteration of the loop
        input_handler_.CheckInput();

        input_handler_.ExecuteInputsCamera();
        camera_.Update();
        input_handler_.ExecuteInputsGame(); 

        // clear the window with black color
        window_.clear(sf::Color::Black);

        //view stuff
        view_ = camera_.GetView();
        window_.setView(view_);

        //Draw game world objects
        DrawGrid(game_grid_);

        // draw ui here...
        window_.setView(window_.getDefaultView());

        // user_interface_.Draw();
        window_.draw(black_square_);
        // window_.draw(tile_preview_sprite_);

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
    grid_view_end_position_ = {
        static_cast<int>(camera_.view_end_position_.x / kTileResolution), 
        static_cast<int>(camera_.view_end_position_.y / kTileResolution)
    };
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
        SetTileSpriteTexture(tile_sprite_, kTileSpritesheet, kTileResolution, tile.GetType());
    }
    tile.type_changed = false; 

    // set position for tile
    tile_sprite_.setPosition({x * kTileResolution * 1.f, y * kTileResolution * 1.f});

    // draw tile
    window_.draw(tile_sprite_);
}


void Game::PlaceTile(sf::Vector2i mouse_position, bool lmb_held) {
    mouse_world_position_ = window_.mapPixelToCoords(mouse_position, camera_.GetView());
    mouse_grid_position_ = sf::Vector2i(
        mouse_world_position_.x / kTileResolution,
        mouse_world_position_.y / kTileResolution
    );
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
    SetTileSpriteTexture(tile_preview_sprite_, kTileSpritesheet, kTileResolution, tile_place_type_);
    return;
}


sf::Time Game::GetDeltaTime() {return delta_time_;}


void Game::SetTileSpriteTexture(sf::Sprite& sprite, const sf::Texture& tile_spritesheet, int kTileResolution, int tile_type) {
    switch (tile_type)
    {
    case 1: //stone
        GetTextureFromSpritesheet(0, 0, tile_spritesheet, kTileResolution, sprite);
        break;
    case 2: //stone_floor
        GetTextureFromSpritesheet(1, 0, tile_spritesheet, kTileResolution, sprite);
        break;
    case 3: //gem
        GetTextureFromSpritesheet(2, 0, tile_spritesheet, kTileResolution, sprite);
        break;
    case 4: //gold
        GetTextureFromSpritesheet(3, 0, tile_spritesheet, kTileResolution, sprite);
        break;
    case 5: //trap
        GetTextureFromSpritesheet(0, 1, tile_spritesheet, kTileResolution, sprite);
        break;
    case 6: //pebbles
        GetTextureFromSpritesheet(2, 1, tile_spritesheet, kTileResolution, sprite);
        break;
    case 7: //crate
        GetTextureFromSpritesheet(1, 1, tile_spritesheet, kTileResolution, sprite);
        break;
    
    default:
        GetTextureFromSpritesheet(0, 0, tile_spritesheet, kTileResolution, sprite);;
        break;
    }
}


void Game::GetTextureFromSpritesheet(int index_x, int index_y, const sf::Texture& spritesheet, 
    int resolution, sf::Sprite& sprite) {

    sprite.setTexture(spritesheet);
    sprite.setTextureRect(sf::IntRect({index_x * resolution, index_y * resolution}, {resolution, resolution}));
}

