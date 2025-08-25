#include "game.hpp"
#include "grid.hpp"
#include "tile.hpp"
#include "camera.hpp"

#include <SFML/Graphics.hpp>

Game::Game(unsigned int window_size_x, unsigned int window_size_y)
  : kWindowSize({window_size_x, window_size_y}),
    kWindowCenter({kWindowSize.x / 2, kWindowSize.y / 2}),

    kTileResolution(8),
    camera(window),
    
    window(sf::VideoMode({kWindowSize.x, kWindowSize.y}), "project"),
    clock(),

    tile_texture(sf::Vector2u(8, 8)),
    tile_spritesheet("sprites/tiles/tiles.png"),

    tile_sprite(tile_texture),
    tile_preview_sprite(tile_texture),

    grid_size(64, 64),
    game_grid(grid_size.x, grid_size.y),

    my_circle(6), 
    tile_place_type(1)
        
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
    while (window.isOpen())
    {        
        delta_time = clock.restart();
        my_circle.setFillColor(sf::Color::Blue);

        // check all the window's events that were triggered since the last iteration of the loop
        mouse_wheel_delta = 0.f;
        CheckEvents();
        SetInputVariables();

        bool inputs[4] = {input_up_held, input_down_held, input_left_held, input_right_held};
        camera.GiveInput(inputs, mouse_wheel_delta, delta_time); 

        HandleTilePlacing();

        camera.HandleGivenInput();
        
        // clear the window with black color
        window.clear(sf::Color::Black);

        //view stuff
        view = camera.GetView();
        window.setView(view);
        camera.SetViewVariables(kTileResolution);

        //Draw game world objects
        DrawGrid(game_grid);

        // draw ui stuff here...
        window.setView(window.getDefaultView());

        window.draw(my_circle);
        window.draw(tile_preview_sprite);

        // end the current frame
        window.display();

    }
}


void Game::CheckEvents() {
    while (const std::optional event = window.pollEvent())
    {
        // "close requested" event: we close the window
        if (event->is<sf::Event::Closed>()){
            window.close();
        }
        
        //Check if mouse moved and calculate mouse positions
        if (const auto* mouse_moved = event->getIf<sf::Event::MouseMoved>()){
            mouse_position = {mouse_moved->position.x, mouse_moved->position.y};
            mouse_world_position = window.mapPixelToCoords(mouse_position, view);
            mouse_grid_position = sf::Vector2i(
                mouse_world_position.x / kTileResolution,
                mouse_world_position.y / kTileResolution
            );
        }

        //check if mouse buttons are pressed (for the first time in a while)
        // rmb_pressed = false;
        // lmb_pressed = false;
        // if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()){
        //     rmb_pressed = mouseButtonPressed->button == sf::Mouse::Button::Right;
        //     lmb_pressed = mouseButtonPressed->button == sf::Mouse::Button::Left;
        // }

        //check mouse wheel movement
        if (const auto* mouseWheelMoved = event->getIf<sf::Event::MouseWheelScrolled>()){
            mouse_wheel_delta = -1 * mouseWheelMoved->delta;
        }

        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
            if (keyPressed->code == sf::Keyboard::Key::Escape){
                window.close();
            }

            if (keyPressed->code == sf::Keyboard::Key::Space){
                spacebar_pressed = true;
                NextTileType();
            }
            
        }
        
    }
}


void Game::DrawGrid(Grid grid) {
    Tile tile;
    // only iterates over tiles within view bounds to improve performance
    // thanks windsurf
    sf::Vector2i view_start_position = camera.view_start_position_;
    sf::Vector2i view_end_position = camera.view_end_position_;
    for (int row = view_start_position.y; row < view_end_position.y; row++) {
        for (int col = view_start_position.x; col < view_end_position.x; col++) {
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
    window.draw(tile_sprite);
}


void Game::SetInputVariables() {
    //movement controls
    input_up_held = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W);
    input_down_held = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S);
    input_left_held = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
    input_right_held = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);

    //mouse controls
    lmb_held = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
    rmb_held = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);

    return;
    
}


void Game::HandleTilePlacing() {
    
    if (lmb_held) {
        my_circle.setFillColor(sf::Color::Green);
        game_grid.SetTile(mouse_grid_position.x, mouse_grid_position.y, tile_place_type);
        last_tile_placed_position = mouse_grid_position;
        return;
    }
    if (rmb_held) {
        my_circle.setFillColor(sf::Color::Red);
        game_grid.SetTile(mouse_grid_position.x, mouse_grid_position.y, "null");
        return;
    }
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

