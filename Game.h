#ifndef GAME_H
#define GAME_H

#define BOARD_SIZE 8
#define FRAME (BOARD_SIZE - 1)
#define DEPTH 3

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>
#include <vector>
#include "Arti.cpp"
#include "Tile.cpp"

using namespace std;
using namespace sf;

enum gameMode_t { PvP, PvC };

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
  Arti artificial_intelligance;

  gameMode_t mode;

  bool initImages();
  bool initTextures();
  void initBoard();
  void chooseMode();

  void updateWindow();

  bool findMoveUp(int row, int col);
  bool findMoveDown(int row, int col);
  bool findMoveRight(int row, int col);
  bool findMoveLeft(int row, int col);
  bool findDiagonRU(int row, int col);
  bool findDiagonRD(int row, int col);
  bool findDiagonLU(int row, int col);
  bool findDiagonLD(int row, int col);

  Vector2i getClickedTile() const;
  void makeMove(int row, int col);
  void clearAvailableMoves();
  bool addPossibleMoves();

  // Helper functions//

  Vector2i getDirection(int src_i, int src_j, int dst_i, int dst_j);
  void changeTextureForPlayer(int row, int col);
  bool findCapturable(int row, int col);
  bool pieceTurn(int row, int col);
  void findWinner();

public:
  Game();
  ~Game();

  bool running();
};

#endif