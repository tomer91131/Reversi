#ifndef GAME_H
#define GAME_H

#define BOARD_SIZE 8
#define FRAME (BOARD_SIZE - 1)
// #define WHITE 0
// #define BLACK 1
#define PRINT_CHECKPOINTS 1
#define checkpoint(arg)                                                        \
  if (PRINT_CHECKPOINTS) {                                                     \
    printf(arg);                                                               \
  }

#include "Tile.cpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>
#include <vector>

using namespace std;
using namespace sf;

class Game {
private:
  RenderWindow *window;
  Image board_image, white_pawn_image, black_pawn_image, valid_square_image;
  Texture board_texture, white_pawn_texture, black_pawn_texture,
      valid_square_texture;
  vector<vector<Tile>> board;
  Sprite board_sprite;
  int player_turn;
  Event event;

  bool initImages();
  bool initTextures();
  void initBoard();

  bool findMoveUp(int row, int col);
  bool findMoveDown(int row, int col);
  bool findMoveRight(int row, int col);
  bool findMoveLeft(int row, int col);
  bool findDiagonRU(int row, int col);
  bool findDiagonRD(int row, int col);
  bool findDiagonLU(int row, int col);
  bool findDiagonLD(int row, int col);

  void makeMove(int row, int col);
  void clearAvailableMoves();
  Vector2i getDirection(int src_i, int src_j, int dst_i, int dst_j);
  void changeTextureForPlayer(int row, int col);
  void findWinner();

public:
  Game();
  ~Game();

  bool running();
  Vector2i getClickedTile() const;
  bool addPossibleMoves();
  void updateWindow();
  bool findCapturable(int row, int col);
  bool pieceTurn(int row, int col);
};

#endif