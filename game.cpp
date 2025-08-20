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
    for (int r = 0; r < grid_size_y; r++) {
        for (int c = 0; c < grid_size_x; c++) {
            //set texture for tile
            if (game_grid.GetTile(c, r).GetType() == "stone") {
                tile_sprite.setTexture(stone_texture);
            } else if (game_grid.GetTile(c, r).GetType() == "stone_floor") {
                tile_sprite.setTexture(stone_floor_texture);
            } else {
                tile_sprite.setTexture(tile_texture);
                std::cerr << "Error: Unknown tile type '" << game_grid.GetTile(c, r).GetType() << "'\n";
                continue; // Skip drawing this tile
            }

            //set position and scale for tile
            tile_sprite.setPosition(sf::Vector2(c * 8.f * zoom_scale, r * zoom_scale * 8.f));
            tile_sprite.setScale({zoom_scale, zoom_scale});
            window.draw(tile_sprite);
        }
    }
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

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here...
        window.draw(tile_sprite);
        
        DrawGrid();

        window.draw(my_circle);

        zoom_scale += clock.getElapsedTime().asSeconds() *0.0001f; // Decrease zoom scale based on elapsed time
        // end the current frame
        window.display();    
    }
}