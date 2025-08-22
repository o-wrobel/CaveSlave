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

    tile_resoultion(8),
    view(sf::FloatRect({0, 0}, {kWindowSize.x * 0.5f, kWindowSize.y * 0.5f})), 
    
    window(sf::VideoMode({kWindowSize.x, kWindowSize.y}), "Cave Slave"),
    clock(),

    tile_texture(sf::Vector2u(8, 8)),

    stone_texture("sprites/stone.png"),
    stone_floor_texture("sprites/stone_floor.png"),
    crate_texture("sprites/crate.png"),

    tile_sprite(tile_texture),

    grid_size(64, 64),
    game_grid(grid_size.x, grid_size.y),

    my_circle(6)
        
{   

    my_circle.setFillColor(sf::Color::Blue);
    my_circle.setOrigin({3.f, 3.f});
    my_circle.setPosition({kWindowSize.x * 0.02f, kWindowSize.y * 0.02f});

}


void Game::DrawGrid(Grid grid) {

    // only iterates over tiles within view bounds to improve performance
    // thanks windsurf

    // for (int row = 0; row < grid_size.y; row++) {
    //     for (int col = 0; col < grid_size.x; col++) {

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
        (x) * tile_resoultion, 
        (y) * tile_resoultion
        ));

        tile.type_changed = false;
    }   

    // draw tile
    window.draw(tile_sprite);
}


void Game::HandleInput() {
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

    //mouse controls
    lmb_pressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
    rmb_pressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
    mouse_position = sf::Mouse::getPosition(window);
    mouse_world_position = window.mapPixelToCoords(mouse_position, view);
    mouse_grid_position = sf::Vector2i(
        mouse_world_position.x / tile_resoultion,
        mouse_world_position.y / tile_resoultion
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


void Game::HandleMouse() {
    if (lmb_pressed) {
        game_grid.SetTile(mouse_grid_position.x, mouse_grid_position.y, "crate");
        my_circle.setFillColor(sf::Color::Green);
    } else if (rmb_pressed) {
        game_grid.SetTile(mouse_grid_position.x, mouse_grid_position.y, "stone_floor");
        my_circle.setFillColor(sf::Color::Red);
    } else {
        my_circle.setFillColor(sf::Color::Blue);
    }
}

void Game::SetViewVariables() {
    view_bounds = view.getViewport();
    view_center = view.getCenter();
    view_size = view.getSize();

    view_start_position = sf::Vector2i(
        view_center.x / tile_resoultion - (view_size.x / tile_resoultion / 2),
        view_center.y / tile_resoultion - (view_size.y / tile_resoultion / 2)
    );

    view_end_position = sf::Vector2i(
        view_start_position.x + (view_size.x / tile_resoultion) + 2,
        view_start_position.y + (view_size.y / tile_resoultion) + 2
    );
}


void Game::GameLoop() {
    // run the program as long as the window is open
    while (window.isOpen())
    {        
        
        // check all the window's events that were triggered since the last iteration of the loop
        while (const std::optional event = window.pollEvent())
        {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        delta_time = clock.restart();

        HandleInput();
        HandleCamera();
        HandleMouse();

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw view stuff here...
        window.setView(view);
        SetViewVariables();

        DrawGrid(game_grid);

        // draw ui stuff here...
        window.setView(window.getDefaultView());
        window.draw(my_circle);

        // end the current frame
        window.display();    
    }
}
