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

    my_circle(5)
        
{   

    my_circle.setFillColor(sf::Color::Red);
    my_circle.setOrigin({5.f, 5.f});
    my_circle.setPosition({kWindowCenter.x * 1.f, kWindowCenter.y * 1.f});

}


void Game::DrawGrid(Grid grid) {
    for (int row = 0; row < grid_size.y; row++) {
        for (int col = 0; col < grid_size.x; col++) {

            DrawTile(grid, col, row);

        }
    }
}


void Game::DrawTile(Grid grid, int x, int y) {

    // 0 for null, 1 for stone, 2 for stone_floor, 3 for crate
    //PLEASE CHANGE THIS LATER TO A HASHTABLE OR SOMETHING

    switch (grid.GetTile(x, y).GetType())
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

    // draw tile
    window.draw(tile_sprite);
}


void Game::HandleInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
        window.close();
    }

    up_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W);
    down_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S);
    left_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
    right_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);

    forward_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up);
    backward_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down);

}


void Game::HandleCamera() {

    view.zoom( 1 + ((backward_pressed - forward_pressed ) * 2) * delta_time.asSeconds());

    int camera_speed =  1 * view.getSize().x;

    view.move(sf::Vector2f(
        ((right_pressed - left_pressed) * camera_speed) * delta_time.asSeconds(), 
        ((down_pressed - up_pressed) * camera_speed) * delta_time.asSeconds()
    ));
    
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

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw view stuff here...
        window.setView(view);
        DrawGrid(game_grid);

        // draw ui stuff here...
        window.setView(window.getDefaultView());
        window.draw(my_circle);

        // end the current frame
        window.display();    
    }
}