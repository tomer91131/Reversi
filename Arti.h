#ifndef ARTI_H
#define ARTI_H

#include <SFML/System/Vector2.hpp>
#include <vector>
#include "Tile.h"

#define BOARD_SIZE 8
#define FRAME (BOARD_SIZE - 1)

using namespace std;
using namespace sf;

class Arti {
private:
  state_t player_turn;

  void makeFakeMove(vector<vector<Tile>> &board, Vector2i tile);
  int findBestMoveAux(vector<vector<Tile>> board, Vector2i move, int depth);
  int rateBoard(vector<vector<Tile>> &board) const;

  Vector2i getDirection(int src_row, int src_col, int dst_row, int dst_col);
  bool pieceTurn(vector<vector<Tile>> &board, int row, int col);
  void clearAvailableMoves(vector<vector<Tile>> &board);
  bool findPossibleMoves(vector<vector<Tile>> &board);
  bool findCapturable(vector<vector<Tile>> &board, int row, int col);
  bool findMoveUp(vector<vector<Tile>> &board, int row, int col);
  bool findMoveDown(vector<vector<Tile>> &board, int row, int col);
  bool findMoveRight(vector<vector<Tile>> &board, int row, int col);
  bool findMoveLeft(vector<vector<Tile>> &board, int row, int col);
  bool findDiagonRU(vector<vector<Tile>> &board, int row, int col);
  bool findDiagonRD(vector<vector<Tile>> &board, int row, int col);
  bool findDiagonLU(vector<vector<Tile>> &board, int row, int col);
  bool findDiagonLD(vector<vector<Tile>> &board, int row, int col);

public:
  Arti() = default;
  ~Arti() = default;
  Vector2i findBestMove(vector<vector<Tile>> &board, int depth);
};

#endif