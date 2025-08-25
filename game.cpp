#include "game.hpp"
#include "grid.hpp"
#include "tile.hpp"
#include "camera.hpp"
#include "input_handler.hpp"

#include <SFML/Graphics.hpp>

Game::Game(unsigned int window_size_x, unsigned int window_size_y)
  : kWindowSize({window_size_x, window_size_y}),
    kWindowCenter({kWindowSize.x / 2, kWindowSize.y / 2}),

    kTileResolution(8),
    
    window_(sf::VideoMode({kWindowSize.x, kWindowSize.y}), "project"),
    camera(window_),
    input_handler_(*this, window_),
    clock(),

    tile_texture(sf::Vector2u(8, 8)),
    tile_spritesheet("sprites/tiles/tiles.png"),

    tile_sprite(tile_texture),
    tile_preview_sprite(tile_texture),

    grid_size(64, 64),
    game_grid(grid_size.x, grid_size.y),

    my_circle(6), 
    tile_place_type(0)
        
{    

    // set position for circle
    my_circle.setFillColor(sf::Color::Blue);
    my_circle.setOrigin({my_circle.getRadius(), my_circle.getRadius()});
    my_circle.setPosition({kWindowSize.x * 0.02f, kWindowSize.y * 0.02f});

    // set position for tile
    SetTileSpriteTexture(tile_preview_sprite, tile_place_type);
    tile_preview_sprite.setOrigin({kTileResolution/2.f, kTileResolution/2.f});
    tile_preview_sprite.setPosition({kWindowSize.x * 0.92f, kWindowSize.y * 0.10f});
    tile_preview_sprite.setColor(sf::Color(255, 255, 255, 200));

    tile_preview_sprite.setScale(sf::Vector2f(6.f, 6.f));

}


void Game::GameLoop() {
    // run the program as long as the window is open
    while (window_.isOpen())
    {        
        delta_time = clock.restart();
        my_circle.setFillColor(sf::Color::Blue);

        CheckEvents(); // check all the window's events that were triggered since the last iteration of the loop
        input_handler_.CheckInput();

        input_handler_.ExecuteInputsCamera();
        camera.Update();
        input_handler_.ExecuteInputsGame();  

        // clear the window with black color
        window_.clear(sf::Color::Black);

        //view stuff
        view = camera.GetView();
        window_.setView(view);

        //Draw game world objects
        DrawGrid(game_grid);

        // draw ui stuff here...
        window_.setView(window_.getDefaultView());

        window_.draw(my_circle);
        window_.draw(tile_preview_sprite);

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
    sf::Vector2f view_start_position = camera.view_start_position_;
    sf::Vector2f view_end_position = camera.view_end_position_;
    grid_view_start_position = {
        static_cast<int>(view_start_position.x / kTileResolution), 
        static_cast<int>(view_start_position.y / kTileResolution)
    };
    grid_view_end_position = {
        static_cast<int>(view_end_position.x / kTileResolution), 
        static_cast<int>(view_end_position.y / kTileResolution)
    };
    for (int row = grid_view_start_position.y; row < grid_view_end_position.y + 1; row++) {
        for (int col = grid_view_start_position.x; col < grid_view_end_position.x + 1; col++) {
            tile = grid.GetTile(col, row);
            if (tile.GetType() != 0){
                DrawTile(tile, col, row);
            }

        }
    }
}


void Game::DrawTile(Tile tile, int x, int y) {
    if (tile.type_changed){
        SetTileSpriteTexture(tile_sprite, tile.GetType());
    }
    // set position for tile
    tile_sprite.setPosition(sf::Vector2f(
    (x) * kTileResolution, 
    (y) * kTileResolution
    ));

    tile.type_changed = false; 

    // draw tile
    window_.draw(tile_sprite);
}


void Game::PlaceTile(sf::Vector2i mouse_position, bool lmb_held) {
    mouse_world_position = window_.mapPixelToCoords(mouse_position, camera.GetView());
    mouse_grid_position = sf::Vector2i(
        mouse_world_position.x / kTileResolution,
        mouse_world_position.y / kTileResolution
    );
    switch (lmb_held) {
    case true:
        game_grid.SetTile(mouse_grid_position.x, mouse_grid_position.y, tile_place_type);
        last_tile_placed_position = mouse_grid_position;
        break;
    case false:
        game_grid.SetTile(mouse_grid_position.x, mouse_grid_position.y, "null");
        break;
    }
    
    last_tile_placed_position = mouse_grid_position;
    return;
}


void Game::NextTileType() {
    tile_place_type++;
    if (tile_place_type >= kTileTypeCount) {
            tile_place_type = 1;
        }
    SetTileSpriteTexture(tile_preview_sprite, tile_place_type);
    return;
}


void Game::SetTileSpriteTexture(sf::Sprite& sprite, int tile_type) {
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
        sprite.setTexture(tile_texture);
        break;
    }
}


void Game::GetTextureFromSpritesheet(int index_x, int index_y, sf::Texture& spritesheet, 
    int resolution, sf::Sprite& sprite) {

    sprite.setTexture(spritesheet);
    sprite.setTextureRect(sf::IntRect({index_x * resolution, index_y * resolution}, {resolution, resolution}));
}

