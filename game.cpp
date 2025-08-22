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

    stone_texture("sprites/stone.png"),
    stone_floor_texture("sprites/stone_floor.png"),
    crate_texture("sprites/crate.png"),
    empty_tile_texture("sprites/empty_tile.png"),

    tile_sprite(tile_texture),
    tile_preview_sprite(stone_texture),

    grid_size(64, 64),
    game_grid(grid_size.x, grid_size.y),

    my_circle(6),
    tile_place_type(0)
        
{   

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
        my_circle.setFillColor(sf::Color::Blue);

        // check all the window's events that were triggered since the last iteration of the loop
        while (const std::optional event = window.pollEvent())
        {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>()){
                window.close();
            }
            
            //check if mouse buttons are pressed (for the first time in a while)
            //Any mouse inputs need to be HERE, not outside the event polling loop
            if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()){
                rmb_clicked = mouseButtonPressed->button == sf::Mouse::Button::Right;
                lmb_clicked = mouseButtonPressed->button == sf::Mouse::Button::Left;
                HandleTilePlacing();
            }
            
        }
        
        delta_time = clock.restart();

        HandleInput();
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
        DrawTilePreview(tile_place_type);

        // end the current frame
        window.display();

    }
}


void Game::DrawGrid(Grid grid) {

    // only iterates over tiles within view bounds to improve performance
    // thanks windsurf
    for (int row = view_start_position.y; row < view_end_position.y; row++) {
        for (int col = view_start_position.x; col < view_end_position.x; col++) {

            DrawTile(grid, col, row);

        }
    }
}


void Game::DrawTile(Grid grid, int x, int y) {

    Tile tile = grid.GetTile(x, y);

    if (tile.type_changed){
        switch (tile.GetType())
        {
        case 1:
            tile_sprite.setTexture(stone_texture);
            break;

        case 2:
            tile_sprite.setTexture(stone_floor_texture);
            break;

        case 3:
            tile_sprite.setTexture(crate_texture);
            break;
        
        default:
            tile_sprite.setTexture(tile_texture);
            break;
        } 

        // set position for tile
        tile_sprite.setPosition(sf::Vector2f(
        (x) * kTileResolution, 
        (y) * kTileResolution
        ));

        tile.type_changed = false;
    }   

    // draw tile
    window.draw(tile_sprite);
}


void Game::DrawTilePreview(int tile_place_type){

    if (true){ // Check if tile_place_type has changed
        switch (tile_place_type)
        {
        case 1:
            tile_preview_sprite.setTexture(stone_texture);
            break;

        case 2:
            tile_preview_sprite.setTexture(stone_floor_texture);
            break;

        case 3:
            tile_preview_sprite.setTexture(crate_texture);
            break;
        
        default:
            tile_preview_sprite.setTexture(empty_tile_texture);
            break;
        }
    }

    // draw tile
    window.draw(tile_preview_sprite);
}

void Game::HandleInput() { //This goes in the event polling loop
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
        window.close();
    }

    //movement controls
    up_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W);
    down_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S);
    left_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
    right_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);

   //camera controls
    forward_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up);
    backward_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down);

    mouse_position = sf::Mouse::getPosition(window);
    mouse_world_position = window.mapPixelToCoords(mouse_position, view);
    mouse_grid_position = sf::Vector2i(
        mouse_world_position.x / kTileResolution,
        mouse_world_position.y / kTileResolution
    );
    
}


void Game::HandleCamera() {

    view.zoom( 1 + ((backward_pressed - forward_pressed ) * 2) * delta_time.asSeconds());
    view_zoom_factor =  window.getSize().x / view.getSize().x;

    int camera_speed =  1 * view.getSize().x;

    view.move(sf::Vector2f(
        ((right_pressed - left_pressed) * camera_speed) * delta_time.asSeconds(), 
        ((down_pressed - up_pressed) * camera_speed) * delta_time.asSeconds()
    ));
    
}


void Game::HandleTilePlacing() {
    
    if (lmb_clicked) {

        game_grid.SetTile(mouse_grid_position.x, mouse_grid_position.y, tile_place_type);
        my_circle.setFillColor(sf::Color::Green);

        return;
    }
    if (rmb_clicked) {
        tile_place_type++;
        if (tile_place_type >= kTileTypeCount) {
            tile_place_type = 0;
        }
        my_circle.setFillColor(sf::Color::Red);

        return;
    }
    
    rmb_clicked = lmb_clicked = false;

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
