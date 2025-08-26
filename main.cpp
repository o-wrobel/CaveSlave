#include <iostream>
#include <SFML/Graphics.hpp>
#include "game.hpp"

int main()
{   // Game Game(640, 480);
    Game Game(1600, 900, 60); // Create an instance of the Game class
    Game.GameLoop(); // Call the run method with specified window size
    
    return 0;
}