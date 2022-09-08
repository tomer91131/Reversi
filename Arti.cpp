#include "Arti.h"

Vector2i Arti::findBestMove(vector<vector<Tile>> &board, int depth) {
  int row, col, best_score = 0;
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      if (board[i][j].getTileState() == VALID_MOVE) {
        player_turn = BLACK;
        int score = findBestMoveAux(board, Vector2i(i, j), depth);
        if (score > best_score) {
          best_score = score;
          row = i;
          col = j;
        }
      }
    }
  }
  return Vector2i(row, col);
}

int Arti::findBestMoveAux(vector<vector<Tile>> board, Vector2i move,
                          int depth) {
  makeFakeMove(board, move);
  if (depth == 1) {
    return rateBoard(board);
  }
  player_turn = (player_turn == WHITE) ? BLACK : WHITE;
  if(!findPossibleMoves(board)){
    return rateBoard(board);
  }
  int best_score = 0;
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      if (board[i][j].getTileState() == VALID_MOVE) {
        int score = findBestMoveAux(board, Vector2i(i, j), depth - 1);
        if (score > best_score) {
          best_score = score;
        }
      }
    }
  }
  return best_score;
}

int Arti::rateBoard(vector<vector<Tile>> &board) const {
  int score = 0;
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      if (board[i][j].getTileState() == BLACK) {
        if ((i == 0 || i == FRAME) && (j == 0 || j == FRAME)) { // Corner
          score += 3;
        } else if (i == 0 || i == FRAME || j == 0 || j == FRAME) // Frame
        {
          score += 2;
        } else if (i == 1 || i == 6 || j == 1 || j == 6) // Inner frame
        {
          continue;
        } else { // Every other case...
          score++;
        }
      }
    }
  }
  return score;
}

void Arti::makeFakeMove(vector<vector<Tile>> &board, Vector2i tile) {
  Vector2i source = board[tile.x][tile.y].pollSource();
  while (source.x != -1) {
    Vector2i direction(getDirection(source.x, source.y, tile.x, tile.y));
    int i = source.x + direction.x, j = source.y + direction.y;
    while (i != tile.x || j != tile.y) {
      board[tile.x][tile.y].setTileState(player_turn);
      i += direction.x;
      j += direction.y;
    }
    source = board[tile.x][tile.y].pollSource();
  }
  board[tile.x][tile.y].setTileState(player_turn);
}

Vector2i Arti::getDirection(int src_row, int src_col, int dst_row,
                            int dst_col) {
  Vector2i vec(0, 0);
  if (src_row < dst_row) {
    vec.x = 1;
  } else if (src_row > dst_row) {
    vec.x = -1;
  }
  if (src_col < dst_col) {
    vec.y = 1;
  } else if (src_col > dst_col) {
    vec.y = -1;
  }
  return vec;
}

/// @brief Answers weather the piece in given place belongs to current player's
/// turn
/// @param row Row coordinate
/// @param col Colum coordinate
bool Arti::pieceTurn(vector<vector<Tile>> &board, int row, int col) {
  if ((board[row][col].getTileState() == WHITE && player_turn == WHITE) ||
      (board[row][col].getTileState() == BLACK && player_turn == BLACK)) {

    return true;
  }
  return false;
}

void Arti::clearAvailableMoves(vector<vector<Tile>> &board) {
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      if (board[i][j].getTileState() == VALID_MOVE) {
        board[i][j].clearSources();
      }
    }
  }
}

bool Arti::findPossibleMoves(vector<vector<Tile>> &board) {
  bool possible_moves = false;
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      if (pieceTurn(board, i, j)) {
        possible_moves = findCapturable(board, i, j) || possible_moves;
      }
    }
  }
  return possible_moves;
}

/// @brief Find all possible moves for given pawn, in all 8 directions
/// @param row Row coordinate of given pawn
/// @param col column coordinate of given pawn
/// @return True if atleast one move was found, False otherwise.
bool Arti::findCapturable(vector<vector<Tile>> &board, int row, int col) {
  bool possible_moves = false;
  possible_moves = findMoveUp(board, row, col) || possible_moves;
  possible_moves = findMoveDown(board, row, col) || possible_moves;
  possible_moves = findMoveRight(board, row, col) || possible_moves;
  possible_moves = findMoveLeft(board, row, col) || possible_moves;
  possible_moves = findDiagonLD(board, row, col) || possible_moves;
  possible_moves = findDiagonLU(board, row, col) || possible_moves;
  possible_moves = findDiagonRD(board, row, col) || possible_moves;
  possible_moves = findDiagonRU(board, row, col) || possible_moves;
  return possible_moves;
}

#pragma region Finding available moves for a pawn functions.

//***********************************************************************//
//*************Functions find the possible moves for a given pawn********//
//***********************************************************************//

bool Arti::findMoveUp(vector<vector<Tile>> &board, int row, int col) {
  int i = row - 1, j = col;
  if (i <= 0 || !board[i][j].isCaptured() || pieceTurn(board, i, j)) {
    return false;
  }
  while (i > 0 && board[i][j].isCaptured() && !pieceTurn(board, i, j)) {
    i--;
  }
  if (!board[i][j].isCaptured()) {
    board[i][j].setTileState(VALID_MOVE);
    board[i][j].addSource(row, col);
    return true;
  }
  return false;
}

bool Arti::findMoveDown(vector<vector<Tile>> &board, int row, int col) {
  int i = row + 1, j = col;
  if (i >= FRAME || !board[i][j].isCaptured() || pieceTurn(board, i, j)) {
    return false;
  }
  while (i < BOARD_SIZE && board[i][j].isCaptured() &&
         !pieceTurn(board, i, j)) {
    i++;
  }
  if (i < BOARD_SIZE && !board[i][j].isCaptured()) {
    board[i][j].setTileState(VALID_MOVE);
    board[i][j].addSource(row, col);
    return true;
  }
  return false;
}

bool Arti::findMoveRight(vector<vector<Tile>> &board, int row, int col) {
  int i = row, j = col + 1;
  if (j >= FRAME || !board[i][j].isCaptured() || pieceTurn(board, i, j)) {
    return false;
  }
  while (j < BOARD_SIZE && board[i][j].isCaptured() &&
         !pieceTurn(board, i, j)) {
    j++;
  }
  if (j < BOARD_SIZE && !board[i][j].isCaptured()) {
    board[i][j].setTileState(VALID_MOVE);
    board[i][j].addSource(row, col);
    return true;
  }
  return false;
}

bool Arti::findMoveLeft(vector<vector<Tile>> &board, int row, int col) {
  int i = row, j = col - 1;
  if (j <= 0 || !board[i][j].isCaptured() || pieceTurn(board, i, j)) {
    return false;
  }
  while (j > 0 && board[i][j].isCaptured() && !pieceTurn(board, i, j)) {
    j--;
  }
  if (!board[i][j].isCaptured()) {
    board[i][j].setTileState(VALID_MOVE);
    board[i][j].addSource(row, col);
    return true;
  }
  return false;
}

bool Arti::findDiagonRU(vector<vector<Tile>> &board, int row, int col) {
  int i = row - 1, j = col + 1;
  if (i <= 0 || j >= FRAME || !board[i][j].isCaptured() ||
      pieceTurn(board, i, j)) {
    return false;
  }
  while (i > 0 && j < BOARD_SIZE && board[i][j].isCaptured() &&
         !pieceTurn(board, i, j)) {
    j++;
    i--;
  }
  if (j < BOARD_SIZE && !board[i][j].isCaptured()) {
    board[i][j].setTileState(VALID_MOVE);
    board[i][j].addSource(row, col);
    return true;
  }
  return false;
}

bool Arti::findDiagonRD(vector<vector<Tile>> &board, int row, int col) {
  int i = row + 1, j = col + 1;
  if (i >= FRAME || j >= FRAME || !board[i][j].isCaptured() ||
      pieceTurn(board, i, j)) {
    return false;
  }
  while (i < BOARD_SIZE && j < BOARD_SIZE && board[i][j].isCaptured() &&
         !pieceTurn(board, i, j)) {
    j++;
    i++;
  }
  if (i < BOARD_SIZE && j < BOARD_SIZE && !board[i][j].isCaptured()) {
    board[i][j].setTileState(VALID_MOVE);
    board[i][j].addSource(row, col);
    return true;
  }
  return false;
}

bool Arti::findDiagonLU(vector<vector<Tile>> &board, int row, int col) {
  int i = row - 1, j = col - 1;
  if (i <= 0 || j <= 0 || !board[i][j].isCaptured() || pieceTurn(board, i, j)) {
    return false;
  }
  while (i > 0 && j > 0 && board[i][j].isCaptured() &&
         !pieceTurn(board, i, j)) {
    j--;
    i--;
  }
  if (!board[i][j].isCaptured()) {
    board[i][j].setTileState(VALID_MOVE);
    board[i][j].addSource(row, col);
    return true;
  }
  return false;
}

bool Arti::findDiagonLD(vector<vector<Tile>> &board, int row, int col) {
  int i = row + 1, j = col - 1;
  if (i >= FRAME || j <= 0 || !board[i][j].isCaptured() ||
      pieceTurn(board, i, j)) {
    return false;
  }
  while (i < BOARD_SIZE && j > 0 && board[i][j].isCaptured() &&
         !pieceTurn(board, i, j)) {
    j--;
    i++;
  }
  if (i < BOARD_SIZE && !board[i][j].isCaptured()) {
    board[i][j].setTileState(VALID_MOVE);
    board[i][j].addSource(row, col);
    return true;
  }
  return false;
}

//***********************************************************************//
#pragma endregion