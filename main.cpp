#include <SFML/Graphics.hpp>

int main()
{   

    const unsigned int WINDOW_SIZE_X = 800;
    const unsigned int WINDOW_SIZE_Y = 600;
    const unsigned int CENTER_X = WINDOW_SIZE_X / 2;
    const unsigned int CENTER_Y = WINDOW_SIZE_Y / 2;
    // create the window
    sf::RenderWindow window(sf::VideoMode({WINDOW_SIZE_X, WINDOW_SIZE_Y}), "My window");

    sf::Texture my_texture("rock.png");

    sf::Sprite my_sprite(my_texture);
    my_sprite.setScale({6, 6});

    sf::CircleShape my_circle(120);
    my_circle.setFillColor(sf::Color::Green);
    my_circle.setPosition({CENTER_X - 120, CENTER_Y - 120});

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