#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <map>

#include "game.hpp"
#include "grid.hpp"
#include "tile.hpp"

Game::Game(unsigned int window_size_x, unsigned int window_size_y)
  : kWindowSize({window_size_x, window_size_y}),
    kWindowCenter({kWindowSize.x / 2, kWindowSize.y / 2}),

    kTileResolution(8),
    view(sf::FloatRect({0, 0}, {kWindowSize.x * 0.5f, kWindowSize.y * 0.5f})), 
    
    window(sf::VideoMode({kWindowSize.x, kWindowSize.y}), "project"),
    clock(),

    tile_texture(sf::Vector2u(8, 8)),

    //Tile preview doesn't work without this for some reason
    //Please fix
    stone_texture("sprites/tiles/stone.png"),
     
    empty_tile_texture("sprites/tiles/empty.png"),

    tile_sprite(tile_texture),
    tile_preview_sprite(stone_texture),

    grid_size(64, 64),
    game_grid(grid_size.x, grid_size.y),

    my_circle(6), 
    tile_place_type(1)
        
{   

    SetTextures();

    my_circle.setFillColor(sf::Color::Blue);
    my_circle.setOrigin({3.f, 3.f});
    my_circle.setPosition({kWindowSize.x * 0.02f, kWindowSize.y * 0.02f});

    // set position for tile
    tile_preview_sprite.setOrigin({4.f, 4.f});
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
        HandleTilePlacing();

        HandleCamera();
        
        // clear the window with black color
        window.clear(sf::Color::Black);

        //view stuff
        window.setView(view);
        SetViewVariables();

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
        rmb_pressed = false;
        lmb_pressed = false;
        if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()){
            rmb_pressed = mouseButtonPressed->button == sf::Mouse::Button::Right;
            lmb_pressed = mouseButtonPressed->button == sf::Mouse::Button::Left;
        }

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


void Game::SetTextures() {
    if (!stone_texture.loadFromFile("sprites/tiles/stone.png")) {
        std::cerr << "Error loading stone texture\n";
    }
    if (!stone_floor_texture.loadFromFile("sprites/tiles/stone_floor.png")) {
        std::cerr << "Error loading stone floor texture\n";
    }
    if (!crate_texture.loadFromFile("sprites/tiles/crate.png")) {
        std::cerr << "Error loading crate texture\n";
    }
    if (!gem_texture.loadFromFile("sprites/tiles/gem.png")) {
        std::cerr << "Error loading gem texture\n";
    }
    if (!gold_texture.loadFromFile("sprites/tiles/gold.png")) {
        std::cerr << "Error loading gem texture\n";
    }
    if (!trap_texture.loadFromFile("sprites/tiles/trap.png")) {
        std::cerr << "Error loading gem texture\n";
    }
    if (!pebbles_texture.loadFromFile("sprites/tiles/pebbles.png")) {
        std::cerr << "Error loading gem texture\n";
    }
    if (!empty_tile_texture.loadFromFile("sprites/tiles/empty.png")) {
        std::cerr << "Error loading empty tile texture\n";
    }
}


void Game::DrawGrid(Grid grid) {
    Tile tile;
    // only iterates over tiles within view bounds to improve performance
    // thanks windsurf
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


void Game::HandleCamera() {
    if (mouse_wheel_delta){
        if ((view_zoom_factor > 1.f || mouse_wheel_delta < 0) && 
            (view_zoom_factor < 10.f || mouse_wheel_delta > 0)) {
            view.zoom(1 + (mouse_wheel_delta * 350) * delta_time.asSeconds());
        }
    }
    view_zoom_factor =  window.getSize().x / view.getSize().x;

    int camera_speed =  1 * view.getSize().x;

    view.move(sf::Vector2f(
        ((input_right_held - input_left_held) * camera_speed) * delta_time.asSeconds(), 
        ((input_down_held - input_up_held) * camera_speed) * delta_time.asSeconds()
    ));
    
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
    case 1:
        sprite.setTexture(stone_texture);
        break;
    case 2:
        sprite.setTexture(stone_floor_texture);
        break;
    case 3:
        sprite.setTexture(gem_texture);
        break;
    case 4:
        sprite.setTexture(gold_texture);
        break;
    case 5:
        sprite.setTexture(trap_texture);
        break;
    case 6:
        sprite.setTexture(pebbles_texture);
        break;
    case 7:
        sprite.setTexture(crate_texture);
        break;
    
    default:
        sprite.setTexture(empty_tile_texture);
        break;
    }
}


void Game::SetViewVariables() {
    view_bounds = view.getViewport();
    view_center = view.getCenter();
    view_size = view.getSize();

    view_start_position = sf::Vector2i(
        view_center.x / kTileResolution - (view_size.x / kTileResolution / 2),
        view_center.y / kTileResolution - (view_size.y / kTileResolution / 2)
    );

    view_end_position = sf::Vector2i(
        view_start_position.x + (view_size.x / kTileResolution) + 2,
        view_start_position.y + (view_size.y / kTileResolution) + 2
    );
}
