#include "camera.hpp"

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>

Camera::Camera(sf::Window& window)
    : window_(window), 
    view_(sf::FloatRect({0, 0}, {window.getSize().x * 1.f, window.getSize().x * 1.f})),
    kWindowSize(sf::Vector2u(window.getSize().x, window.getSize().y)),
    zoom_factor_(1.f)
{
    view_.setCenter({window.getSize().x * 0.5f, window.getSize().x * 0.5f});
}


void Camera::Update(){
    move_speed_ =  1 * view_.getSize().x;
    zoom_factor_ = std::clamp(zoom_factor_, 0.8f, 7.0f);
    view_.setSize(sf::Vector2f(kWindowSize.x / pow(2, zoom_factor_ - 1), kWindowSize.y / pow(2, zoom_factor_ - 1)));
    view_.setCenter(position_);

    sf::Vector2f view_size = view_.getSize();

    view_start_position_ = position_ - view_size * 0.5f; 
    view_end_position_ = view_start_position_ + view_size;
}


sf::View Camera::GetView(){
    return view_;
}
