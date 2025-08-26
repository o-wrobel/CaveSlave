#include <SFML/Graphics.hpp>
#include "user_interface.hpp"
#include "game.hpp"

UserInterface::UserInterface(Game& game)
    : game_(game),
    window_(game_.window_),

    tile_texture_(sf::Vector2u(8, 8)),
    tile_preview_sprite_(tile_texture_)
{

}