#include <iostream>
#include <filesystem>
#include <SFML/Graphics.hpp>

#include "game.hpp"

Game::Game(int window_size_x, int window_size_y)
  : WINDOW_SIZE_X(window_size_x),
    WINDOW_SIZE_Y(window_size_y),
    CENTER_X(window_size_x / 2),
    CENTER_Y(window_size_y / 2),
    window(sf::VideoMode({WINDOW_SIZE_X, WINDOW_SIZE_Y}), "Cave Slave"),
    my_texture("sprites/rock.png"),
    my_sprite(my_texture),
    my_circle(120)
        
{
    my_sprite.setTexture(my_texture);
    my_sprite.setScale({4, 4});

    my_circle.setFillColor(sf::Color::Green);
    my_circle.setPosition({CENTER_X - 120.f, CENTER_Y - 120.f});
}


void Game::run() {

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
        window.draw(my_sprite);
        window.draw(my_circle);

        // end the current frame
        window.display();    
    }
}