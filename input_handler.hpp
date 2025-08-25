#ifndef INPUT_HANDLER_HPP
#define INPUT_HANDLER_HPP

#include <SFML/Graphics.hpp>

#include <optional>

class Game;

class InputHandler {
private:
    Game& game_;
    sf::Window& window_;

    // bool up_held;
    // bool down_held;
    // bool left_held;
    // bool right_held;

    bool spacebar_pressed;

    bool lmb_held;
    bool rmb_held;

    float mouse_wheel_delta;

    sf::Vector2i mouse_position;

public:
    bool up_held;
    bool down_held;
    bool left_held;
    bool right_held;
    
    InputHandler();
    InputHandler(Game& game, sf::Window& window);
    void CheckInput();
    void CheckEventInput(const std::optional<sf::Event> event);
};

#endif