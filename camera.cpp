#include "game.hpp"
#include "camera.hpp"

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>

Camera::Camera(Game& game)
    : game_(game),
    window_(game_.window_), 
    view_(sf::FloatRect({0, 0}, {window_.getSize().x * 1.f, window_.getSize().x * 1.f})),
    kWindowSize(sf::Vector2u(window_.getSize().x, window_.getSize().y)),
    zoom_factor_(1.f)
{
    view_.setCenter({window_.getSize().x * 0.5f, window_.getSize().x * 0.5f});
}


void Camera::Update(){
    delta_time_ = game_.GetDeltaTime();

    move_speed_ =  1 * view_.getSize().x;
    zoom_factor_ = std::clamp(zoom_factor_, 0.8f, 7.0f);
    view_.setSize(sf::Vector2f(kWindowSize.x / pow(2, zoom_factor_ - 1), kWindowSize.y / pow(2, zoom_factor_ - 1)));
    view_.setCenter(position_);

    sf::Vector2f view_size = view_.getSize();

    view_start_position_ = position_ - view_size * 0.5f; 
    view_end_position_ = view_start_position_ + view_size;
}


void Camera::MoveCamera(sf::Vector2f offset){
    position_ += offset * move_speed_ * delta_time_.asSeconds();
}

void Camera::ChangeZoom(float zoom_offset){
    zoom_factor_ += zoom_offset;
}


sf::View Camera::GetView(){
    return view_;
}
