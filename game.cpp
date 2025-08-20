#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>

#include "game.hpp"
#include "grid.hpp"
#include "tile.hpp"

Game::Game(int window_size_x, int window_size_y)
  : WINDOW_SIZE_X(window_size_x),
    WINDOW_SIZE_Y(window_size_y),
    CENTER_X(window_size_x / 2),
    CENTER_Y(window_size_y / 2),
    
    window(sf::VideoMode({WINDOW_SIZE_X, WINDOW_SIZE_Y}), "Cave Slave"),
    clock(),

    camera_position({0, 0}),
    zoom_scale(3),

    tile_texture(sf::Vector2u(8, 8)), 
    stone_texture("sprites/stone.png"),
    stone_floor_texture("sprites/stone_floor.png"),
    tile_sprite(tile_texture),

    grid_size_x(20),
    grid_size_y(20),
    game_grid(grid_size_x, grid_size_y),

    my_circle(120)
        
{
    // tile_sprite.setTexture(stone_texture);
    

    my_circle.setFillColor(sf::Color::Green);
    my_circle.setPosition({CENTER_X - 120.f, CENTER_Y - 120.f});
}


void Game::DrawGrid() {
    for (int row = 0; row < grid_size_y; row++) {
        for (int col = 0; col < grid_size_x; col++) {
            // int row = (grid_size_y - 1) - row;

            //set texture for tile
            if (game_grid.GetTile(col, row).GetType() == "stone") {
                tile_sprite.setTexture(stone_texture);
            } else if (game_grid.GetTile(col, row).GetType() == "stone_floor") {
                tile_sprite.setTexture(stone_floor_texture);
            } else {
                tile_sprite.setTexture(tile_texture);
                std::cerr << "Error: Unknown tile type '" << game_grid.GetTile(col, row).GetType() << "'\n";
                continue; // Skip drawing this tile
            }

            //set position and scale for tile
            tile_sprite.setPosition(sf::Vector2(col * zoom_scale * 8.f  + camera_position.x, row * zoom_scale * 8.f + camera_position.y));
            tile_sprite.setScale({zoom_scale, zoom_scale});
            window.draw(tile_sprite);
        }
    }
}

void Game::HandleInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
        window.close();
    }

    
    up_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W);
    down_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S);
    left_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
    right_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);

    camera_position.x += ((right_pressed - left_pressed) * 100) * delta_time.asSeconds(); // 
    camera_position.y += ((down_pressed - up_pressed) * 100) * delta_time.asSeconds(); //
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

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here...
        window.draw(tile_sprite);
        
        DrawGrid();

        window.draw(my_circle);

        // end the current frame

        window.display();    
    }
}