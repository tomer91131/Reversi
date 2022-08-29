#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>
#include <vector>

using namespace std;
using namespace sf;

class Game
{
private:
    Window* window;
public:
    Game(/* args */);
    ~Game();
};



#endif