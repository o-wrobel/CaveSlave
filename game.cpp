#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>

#include "game.hpp"
#include "grid.hpp"
#include "tile.hpp"

Game::Game(unsigned int window_size_x, unsigned int window_size_y)
  : kWindowSize({window_size_x, window_size_y}),
    kWindowCenter({kWindowSize.x / 2, kWindowSize.y / 2}),

    tile_resoultion(8),
    
    window(sf::VideoMode({kWindowSize.x, kWindowSize.y}), "Cave Slave"),
    clock(),

    camera_position({0, 0}),
    zoom_scale(2),

    tile_texture(sf::Vector2u(8, 8)),

    stone_texture("sprites/stone.png"),
    stone_floor_texture("sprites/stone_floor.png"),
    crate_texture("sprites/crate.png"),

    tile_sprite(tile_texture),

    grid_size_x(32),
    grid_size_y(32),
    game_grid(grid_size_x, grid_size_y),

    my_circle(5)
        
{
    my_circle.setFillColor(sf::Color::Red);
    my_circle.setOrigin({5.f, 5.f});
    my_circle.setPosition({kWindowCenter.x *1.f, kWindowCenter.y * 1.f});
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
            } else if (game_grid.GetTile(col, row).GetType() == "crate") {
                tile_sprite.setTexture(crate_texture);
            } else {
                tile_sprite.setTexture(tile_texture);
                std::cerr << "Error: Unknown tile type '" << game_grid.GetTile(col, row).GetType() << "'\n";
                continue; // Skip drawing this tile
            }

            // set position and scale for tile
            tile_sprite.setPosition(sf::Vector2(
                (col * zoom_scale - camera_position.x) * tile_resoultion, 
                (row * zoom_scale - camera_position.y) * tile_resoultion
            ));



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

    forward_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up);
    backward_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down);

    camera_position.x += ((right_pressed - left_pressed) * 100) * delta_time.asSeconds(); // 
    camera_position.y += ((down_pressed - up_pressed) * 100) * delta_time.asSeconds(); //

    zoom_scale += ((forward_pressed - backward_pressed) * 4) * delta_time.asSeconds();
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