#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>
#include <vector>

using namespace std;
using namespace sf;

#define BOARD_SIZE 8
#define WHITE 0
#define BLACK 1
#define PRINT_CHECKPOINTS 1
#define checkpoint(arg) if(PRINT_CHECKPOINTS){printf(arg);}

void initBoard(vector<vector<Sprite*>> &board,Texture &black_pawn,Texture &white_pawn){
    checkpoint("initializing board\n")
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        board.push_back(vector<Sprite*>(8,nullptr));
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            //checkpoint("reached i=%d j=%d\n",i,j)
            board[i][j] = new Sprite();
            board[i][j]->setPosition(100*i,100*j);
            
        }
    }
    checkpoint("success\nsetting first pawns\n")
    board[3][3]->setTexture(white_pawn);
    board[3][4]->setTexture(black_pawn);
    board[4][4]->setTexture(white_pawn);
    board[4][3]->setTexture(black_pawn);
    checkpoint("success\n")
}

void updateWindow(RenderWindow &window, vector<vector<Sprite*>> &board, Texture &black_pawn,Texture &white_pawn,Sprite &board_sprite){
    window.clear();
    window.draw(board_sprite);
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if(board[i][j]->getTexture() == &black_pawn || board[i][j]->getTexture() == &white_pawn){
                window.draw(*board[i][j]);
                printf("i = %d j =%d\n",i,j);
            }
        }
    }
    checkpoint("finished drawing-> displaying\n")
    window.display();
}

Vector2i getClickedTile(RenderWindow &window){
    Vector2i curser_position = Mouse::getPosition(window);
    curser_position.x = curser_position.x/BOARD_SIZE;
    curser_position.y = curser_position.y/BOARD_SIZE;
    return curser_position;
}

int main()
{
    checkpoint("starting to build game\n")
    RenderWindow window(VideoMode(800, 800), "Reversi");
    window.setFramerateLimit(4);
    Image board_image,white_pawn_image,black_pawn_image,valid_square_image;
    Texture board_texture,white_pawn_texture,black_pawn_texture,valid_square_texture;
    vector<vector<Sprite*>> board;
    Sprite board_sprite;
    int player_turn = WHITE;

    checkpoint("loading images\n")
    if(!(board_image.loadFromFile("images/board.jpg") 
            && black_pawn_image.loadFromFile("images/black_pawn.jpg")
            && white_pawn_image.loadFromFile("images/white_pawn.jpg") 
            && valid_square_image.loadFromFile("images/valid_square.jpg"))){
        printf("failed to load the images");
        return 0;
    }
    checkpoint("success\nloading textures\n")

    board_texture.loadFromImage(board_image);
    white_pawn_texture.loadFromImage(white_pawn_image);
    black_pawn_texture.loadFromImage(black_pawn_image);
    valid_square_texture.loadFromImage(valid_square_image);
    board_sprite.setTexture(board_texture);

    initBoard(board,black_pawn_texture,white_pawn_texture);
    updateWindow(window,board,black_pawn_texture,white_pawn_texture,board_sprite);
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::MouseButtonPressed){
                updateWindow(window,board,black_pawn_texture,white_pawn_texture,board_sprite);
            }
        }
    }

    return 0;
}