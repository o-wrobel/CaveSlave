#include <iostream>
#include <SFML/Graphics.hpp>
#include "game.hpp"

int main()
{   
    Game Game(720, 720); // Create an instance of the Game class
    Game.GameLoop(); // Call the run method with specified window size
    
}