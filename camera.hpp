#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <SFML/Graphics.hpp>

class Camera {
private:
    sf::Window& window_;
    sf::View view_;
    sf::Vector2f position_;
    
    const sf::Vector2i kWindowSize;

    float zoom_factor_;
    float move_speed_;
    bool up_held_, down_held_, left_held_, right_held_; 
    float mouse_wheel_delta_;

    sf::Time delta_time_;

public:
    sf::Vector2i view_start_position_;
    sf::Vector2i view_end_position_;

    Camera(sf::Window& window); //, sf::Vector2f position, sf::Vector2f size
    void GiveInput(bool inputs[4], float mouse_wheel_delta, sf::Time delta_time);
    sf::View GetView();
    void SetViewVariables(int tile_resolution);
    void SetPosition(sf::Vector2i position);
    void HandleGivenInput();
};
#endif