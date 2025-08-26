#include "input_handler.hpp"
#include "game.hpp"
#include "camera.hpp"
#include "user_interface.hpp"

#include <SFML/Graphics.hpp>

InputHandler::InputHandler(Game& game)
    : game_(game), 
    window_(game_.window_) 
{

}


void InputHandler::CheckInput() {
    //movement controls
    up_held_ = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W);
    down_held_ = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S);
    left_held_ = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
    right_held_ = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);

    //mouse controls
    lmb_held_ = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
    rmb_held_ = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);

    return;
    
}


void InputHandler::CheckEventInput(const std::optional<sf::Event> event) {
    //Check if mouse moved and calculate mouse positions
    if (const auto* mouse_moved = event->getIf<sf::Event::MouseMoved>()){
        mouse_position_ = {mouse_moved->position.x, mouse_moved->position.y};
        
    }

    //check mouse wheel movement
    if (const auto* mouseWheelMoved = event->getIf<sf::Event::MouseWheelScrolled>()){
        mouse_wheel_delta_ = mouseWheelMoved->delta;
    }

    if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
        if (keyPressed->code == sf::Keyboard::Key::Escape){
            window_.close();
        }

        if (keyPressed->code == sf::Keyboard::Key::Space){
            spacebar_pressed_ = true;
        }
        
    }
}


void InputHandler::ExecuteInputsCamera() {
    if (mouse_wheel_delta_){game_.camera_.ChangeZoom(mouse_wheel_delta_ * 0.5f);} //game_.camera_.zoom_factor_ -= mouse_wheel_delta_ * 0.5f;
    mouse_wheel_delta_ = 0;

    if(right_held_ || left_held_ || up_held_ || down_held_){
        game_.camera_.MoveCamera({right_held_-left_held_ * 1.f, down_held_-up_held_ * 1.f});
    }
}


void InputHandler::ExecuteInputsGame() {
    if(spacebar_pressed_){
        game_.NextTilePlaceType();
        game_.user_interface_.NextTilePlaceType();
    } 
    spacebar_pressed_ = false;

    if (lmb_held_) {
        game_.user_interface_.my_circle_.setFillColor(sf::Color::Green);
        game_.PlaceTile(mouse_position_, true);
        return;
    }
    if (rmb_held_) {
        game_.user_interface_.my_circle_.setFillColor(sf::Color::Red);
        game_.PlaceTile(mouse_position_, false);
        return;
    }
}
