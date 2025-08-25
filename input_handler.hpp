#ifndef INPUT_HANDLER_HPP
#define INPUT_HANDLER_HPP

#include <SFML/Graphics.hpp>

#include <optional>

class Game;

class InputHandler {
private:
    Game& game_;
    sf::Window& window_;

    bool up_held_;
    bool down_held_;
    bool left_held_;
    bool right_held_;
    
    bool spacebar_pressed_;

    bool lmb_held_;
    bool rmb_held_;

    float mouse_wheel_delta_;

    sf::Vector2i mouse_position_;

public:
    

    InputHandler(); // Default constructor // neccessary for some reason
    InputHandler(Game& game, sf::Window& window);
    void CheckInput();
    void CheckEventInput(const std::optional<sf::Event> event);
    void ExecuteInputsCamera();
    void ExecuteInputsGame();

};

#endif