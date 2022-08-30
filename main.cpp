#include "Game.cpp"

using namespace std;
using namespace sf;

#define BOARD_SIZE 8
#define WHITE 0
#define BLACK 1
#define PRINT_CHECKPOINTS 1
#define checkpoint(arg) if(PRINT_CHECKPOINTS){printf(arg);}

int main()
{
    Game game;
    while (game.running())
    {
        
    }

    return 0;
}