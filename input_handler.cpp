#include "input_handler.hpp"
#include "game.hpp"

#include <SFML/Graphics.hpp>

InputHandler::InputHandler(Game& game, sf::Window& window) : game_(game), window_(window) {
    
}

void InputHandler::CheckInput() {
    //movement controls
    up_held = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W);
    down_held = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S);
    left_held = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
    right_held = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);

    //mouse controls
    lmb_held = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
    rmb_held = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);

    return;
    
}

void InputHandler::CheckEventInput(const std::optional<sf::Event> event) {
    //Check if mouse moved and calculate mouse positions
    if (const auto* mouse_moved = event->getIf<sf::Event::MouseMoved>()){
        mouse_position = {mouse_moved->position.x, mouse_moved->position.y};
        
    }

    //check mouse wheel movement
    if (const auto* mouseWheelMoved = event->getIf<sf::Event::MouseWheelScrolled>()){
        mouse_wheel_delta = -1 * mouseWheelMoved->delta;
    }

    if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
        if (keyPressed->code == sf::Keyboard::Key::Escape){
            window_.close();
        }

        if (keyPressed->code == sf::Keyboard::Key::Space){
            spacebar_pressed = true;
            game_.NextTileType();
        }
        
    }
}
