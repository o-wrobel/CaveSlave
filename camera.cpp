#include "camera.hpp"

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>

Camera::Camera(sf::Window& window)
    : window_(window), 
    view_(sf::FloatRect({0, 0}, {window.getSize().x * 1.f, window.getSize().x * 1.f})),
    kWindowSize(sf::Vector2u(window.getSize().x, window.getSize().y)),
    zoom_factor_(2.f)
{
    view_.setCenter({window.getSize().x * 0.5f, window.getSize().x * 0.5f});
}


void Camera::GiveInput(bool inputs[4],float mouse_wheel_delta, sf::Time delta_time){
    up_held_ = inputs[0];
    down_held_ = inputs[1];
    left_held_ = inputs[2];
    right_held_ = inputs[3];
    mouse_wheel_delta_ = mouse_wheel_delta;
    delta_time_ = delta_time;
}


void Camera::HandleGivenInput(){
    if (mouse_wheel_delta_){
        zoom_factor_ -= mouse_wheel_delta_ * 0.5f;
        zoom_factor_ = std::clamp(zoom_factor_, 0.8f, 7.0f);
    }
    view_.setSize(sf::Vector2f(kWindowSize.x / pow(2, zoom_factor_ - 1), kWindowSize.y / pow(2, zoom_factor_ - 1)));

    move_speed_ =  1 * view_.getSize().x;
    position_ += sf::Vector2f(
        ((right_held_ - left_held_) * move_speed_) * delta_time_.asSeconds(), 
        ((down_held_ - up_held_) * move_speed_) * delta_time_.asSeconds()
    );
    view_.setCenter(position_);

}


void Camera::SetViewVariables(int tile_resolution){
    sf::FloatRect view_bounds = view_.getViewport();
    sf::Vector2f view_center = view_.getCenter();
    sf::Vector2f view_size = view_.getSize();

    view_start_position_ = sf::Vector2i(
        view_center.x / tile_resolution - (view_size.x / tile_resolution / 2),
        view_center.y / tile_resolution - (view_size.y / tile_resolution / 2)
    );

    view_end_position_ = sf::Vector2i(
        view_start_position_.x + (view_size.x / tile_resolution) + 2,
        view_start_position_.y + (view_size.y / tile_resolution) + 2
    );
}


sf::View Camera::GetView(){
    return view_;
}
