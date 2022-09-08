#include "Game.h"

//***********************************************************************//
//******************************Public Methods***************************//
//***********************************************************************//
Game::Game() {
  window = new RenderWindow(VideoMode(800, 800), "Reversi");
  window->setFramerateLimit(4);
  if (!initImages()) {
    printf("failed to load the images");
  }
  if (!initTextures()) {
    printf("failed to load textures");
  }
  player_turn = WHITE;
  initBoard();
  chooseMode();
  
  addPossibleMoves();
  updateWindow();
}

Game::~Game() { delete window; }

/// @brief Event managing function, controlling the state of the game
/// @return Weather the game is still going
bool Game::running() {
  while (window->pollEvent(event)) {
    switch (event.type) {
    case Event::Closed:
      printf("closing");
      window->close();
      break;
    case Event::MouseButtonPressed:
      Vector2i tile(getClickedTile());
      if (tile.x > FRAME || tile.y > FRAME) {
        break;
      }
      if (board[tile.x][tile.y].getTileState() == VALID_MOVE) {
        makeMove(tile.x, tile.y);
        clearAvailableMoves();
        player_turn = (player_turn == WHITE) ? BLACK : WHITE;
        if (!addPossibleMoves()) {
          printf("game ended!\n");
          findWinner();
        }
        if(mode == PvC && player_turn == BLACK){
          Vector2i move = artificial_intelligance.findBestMove(board,DEPTH);
          makeMove(move.x, move.y);
          clearAvailableMoves();
          player_turn = WHITE;
          if (!addPossibleMoves()) {
            printf("game ended!\n");
            findWinner();
          }
        }
        updateWindow();
      }
      break;
    }
  }
  return window->isOpen();
}

//***********************************************************************//
//*****************************Private Methods***************************//
//***********************************************************************//

/// @brief Load images
/// @return If the action succeeds
bool Game::initImages() {
  if (!(board_image.loadFromFile("images/board.jpg") &&
        black_pawn_image.loadFromFile("images/black_pawn.jpg") &&
        white_pawn_image.loadFromFile("images/white_pawn.jpg") &&
        valid_square_image.loadFromFile("images/valid_square.jpg"))) {
    return false;
  }
  return true;
}

/// @brief Load images into textures
/// @return If the action succeeds
bool Game::initTextures() {
  if (!(board_texture.loadFromImage(board_image) &&
        white_pawn_texture.loadFromImage(white_pawn_image) &&
        black_pawn_texture.loadFromImage(black_pawn_image) &&
        valid_square_texture.loadFromImage(valid_square_image))) {
    return false;
  }
  board_sprite.setTexture(board_texture);
  return true;
}

/// @brief Initialize the board to start the game
void Game::initBoard() {
  for (int i = 0; i < BOARD_SIZE; i++) {
    board.emplace_back(vector<Tile>());
    for (int j = 0; j < BOARD_SIZE; j++) {
      board[i].emplace_back(Tile());
      board[i][j].setPosition(100 * j, 100 * i);
      board[i][j].setTileState(EMPTY);
    }
  }
  board[3][3].setTileTexture(white_pawn_texture, WHITE);
  board[3][4].setTileTexture(black_pawn_texture, BLACK);
  board[4][4].setTileTexture(white_pawn_texture, WHITE);
  board[4][3].setTileTexture(black_pawn_texture, BLACK);
}

void Game::chooseMode() {
  Font font;
  if (!font.loadFromFile("Font.ttf")) {
    printf("failed to load font");
    return;
  }
  Text headline("Choose game mode: RED (PvP) || BLUE(PvC) ", font);
  RectangleShape PvP(Vector2f(100, 100));
  RectangleShape PvC(Vector2f(100, 100));
  headline.setPosition(0, 0);
  PvP.setPosition(0, 100);
  PvC.setPosition(100, 100);
  PvP.setFillColor(Color(255, 0, 0));
  PvC.setFillColor(Color(0, 0, 255));
  window->clear();
  window->draw(headline);
  window->draw(PvP);
  window->draw(PvC);
  window->display();

  bool cond = false;
  while (true) {
    cond = window->pollEvent(event);
    if (cond && event.type == Event::MouseButtonPressed) {
      Vector2i coordinates = getClickedTile();
      if (coordinates.x == 1) {
        if (coordinates.y == 0) {
          mode = gameMode_t::PvP;
          break;
        } else if (coordinates.y == 1) {
          mode = gameMode_t::PvC;
          break;
        }
        cond = false;
      }
    }
    if (cond && event.type == Event::Closed) {
      window->close();
      return;
    }
  }
}

/// @brief Display the new board state
void Game::updateWindow() {
  window->clear();
  window->draw(board_sprite);
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      if (board[i][j].getTileState() != EMPTY) {
        window->draw(board[i][j]);
      }
    }
  }
  window->display();
}

#pragma region Finding available moves for a pawn functions.

//***********************************************************************//
//*************Functions find the possible moves for a given pawn********//
//***********************************************************************//

bool Game::findMoveUp(int row, int col) {
  int i = row - 1, j = col;
  if (i <= 0 || !board[i][j].isCaptured() || pieceTurn(i, j)) {
    return false;
  }
  while (i > 0 && board[i][j].isCaptured() && !pieceTurn(i, j)) {
    i--;
  }
  if (!board[i][j].isCaptured()) {
    board[i][j].setTileTexture(valid_square_texture, VALID_MOVE);
    board[i][j].addSource(row, col);
    return true;
  }
  return false;
}
bool Game::findMoveDown(int row, int col) {
  int i = row + 1, j = col;
  if (i >= FRAME || !board[i][j].isCaptured() || pieceTurn(i, j)) {
    return false;
  }
  while (i < BOARD_SIZE && board[i][j].isCaptured() && !pieceTurn(i, j)) {
    i++;
  }
  if (i < BOARD_SIZE && !board[i][j].isCaptured()) {
    board[i][j].setTileTexture(valid_square_texture, VALID_MOVE);
    board[i][j].addSource(row, col);
    return true;
  }
  return false;
}
bool Game::findMoveRight(int row, int col) {
  int i = row, j = col + 1;
  if (j >= FRAME || !board[i][j].isCaptured() || pieceTurn(i, j)) {
    return false;
  }
  while (j < BOARD_SIZE && board[i][j].isCaptured() && !pieceTurn(i, j)) {
    j++;
  }
  if (j < BOARD_SIZE && !board[i][j].isCaptured()) {
    board[i][j].setTileTexture(valid_square_texture, VALID_MOVE);
    board[i][j].addSource(row, col);
    return true;
  }
  return false;
}
bool Game::findMoveLeft(int row, int col) {
  int i = row, j = col - 1;
  if (j <= 0 || !board[i][j].isCaptured() || pieceTurn(i, j)) {
    return false;
  }
  while (j > 0 && board[i][j].isCaptured() && !pieceTurn(i, j)) {
    j--;
  }
  if (!board[i][j].isCaptured()) {
    board[i][j].setTileTexture(valid_square_texture, VALID_MOVE);
    board[i][j].addSource(row, col);
    return true;
  }
  return false;
}
bool Game::findDiagonRU(int row, int col) {
  int i = row - 1, j = col + 1;
  if (i <= 0 || j >= FRAME || !board[i][j].isCaptured() || pieceTurn(i, j)) {
    return false;
  }
  while (i > 0 && j < BOARD_SIZE && board[i][j].isCaptured() &&
         !pieceTurn(i, j)) {
    j++;
    i--;
  }
  if (j < BOARD_SIZE && !board[i][j].isCaptured()) {
    board[i][j].setTileTexture(valid_square_texture, VALID_MOVE);
    board[i][j].addSource(row, col);
    return true;
  }
  return false;
}
bool Game::findDiagonRD(int row, int col) {
  int i = row + 1, j = col + 1;
  if (i >= FRAME || j >= FRAME || !board[i][j].isCaptured() ||
      pieceTurn(i, j)) {
    return false;
  }
  while (i < BOARD_SIZE && j < BOARD_SIZE && board[i][j].isCaptured() &&
         !pieceTurn(i, j)) {
    j++;
    i++;
  }
  if (i < BOARD_SIZE && j < BOARD_SIZE && !board[i][j].isCaptured()) {
    board[i][j].setTileTexture(valid_square_texture, VALID_MOVE);
    board[i][j].addSource(row, col);
    return true;
  }
  return false;
}
bool Game::findDiagonLU(int row, int col) {
  int i = row - 1, j = col - 1;
  if (i <= 0 || j <= 0 || !board[i][j].isCaptured() || pieceTurn(i, j)) {
    return false;
  }
  while (i > 0 && j > 0 && board[i][j].isCaptured() && !pieceTurn(i, j)) {
    j--;
    i--;
  }
  if (!board[i][j].isCaptured()) {
    board[i][j].setTileTexture(valid_square_texture, VALID_MOVE);
    board[i][j].addSource(row, col);
    return true;
  }
  return false;
}
bool Game::findDiagonLD(int row, int col) {
  int i = row + 1, j = col - 1;
  if (i >= FRAME || j <= 0 || !board[i][j].isCaptured() || pieceTurn(i, j)) {
    return false;
  }
  while (i < BOARD_SIZE && j > 0 && board[i][j].isCaptured() &&
         !pieceTurn(i, j)) {
    j--;
    i++;
  }
  if (i < BOARD_SIZE && !board[i][j].isCaptured()) {
    board[i][j].setTileTexture(valid_square_texture, VALID_MOVE);
    board[i][j].addSource(row, col);
    return true;
  }
  return false;
}
//***********************************************************************//
#pragma endregion

/// @brief Calculate the clicked tile on board
/// @return Coordinates of clicked tile
Vector2i Game::getClickedTile() const {
  Vector2i vec(sf::Mouse::getPosition(*window));
  int tmp = vec.x;
  vec.x = vec.y / 100;
  vec.y = tmp / 100;
  return vec;
}

/// @brief Capture a given tile from all possible directions
/// @param row Row coordinates of captured tile
/// @param col Column coordinates of captured tile
void Game::makeMove(int row, int col) {
  Vector2i source = board[row][col].pollSource();
  while (source.x != -1) {
    Vector2i direction(getDirection(source.x, source.y, row, col));
    int i = source.x + direction.x, j = source.y + direction.y;
    while (i != row || j != col) {
      changeTextureForPlayer(i, j);
      i += direction.x;
      j += direction.y;
    }
    source = board[row][col].pollSource();
  }
  changeTextureForPlayer(row, col);
}

/// @brief Clear the board from old irrelevent moves
void Game::clearAvailableMoves() {
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      if (board[i][j].getTileState() == VALID_MOVE) {
        board[i][j].clearSources();
      }
    }
  }
}

/// @brief adds possible moves for all the pawns which are in turn
/// @return True if atleast one move was found, False otherwise(game ends).
bool Game::addPossibleMoves() {
  bool possible_moves = false;
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      if (pieceTurn(i, j)) {
        possible_moves = findCapturable(i, j) || possible_moves;
      }
    }
  }
  return possible_moves;
}

//***********************************************************************//
//*****************************Helper functions**************************//
//***********************************************************************//

/// @brief Calculate the direction of approach from one tile to the other,
///        destination MUST be in one of the conventional directions.
/// @return Direction vector to destination
Vector2i Game::getDirection(int src_row, int src_col, int dst_row,
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

/// @brief Change the texture according to current player's turn
/// @param row Row coordinate
/// @param col Column coordinate
void Game::changeTextureForPlayer(int row, int col) {
  if (player_turn == WHITE) {
    board[row][col].setTileTexture(white_pawn_texture, WHITE);
  } else {
    board[row][col].setTileTexture(black_pawn_texture, BLACK);
  }
}

/// @brief Find all possible moves for given pawn, in all 8 directions
/// @param row Row coordinate of given pawn
/// @param col column coordinate of given pawn
/// @return True if atleast one move was found, False otherwise.
bool Game::findCapturable(int row, int col) {
  bool possible_moves = false;
  possible_moves = findMoveUp(row, col) || possible_moves;
  possible_moves = findMoveDown(row, col) || possible_moves;
  possible_moves = findMoveRight(row, col) || possible_moves;
  possible_moves = findMoveLeft(row, col) || possible_moves;
  possible_moves = findDiagonLD(row, col) || possible_moves;
  possible_moves = findDiagonLU(row, col) || possible_moves;
  possible_moves = findDiagonRD(row, col) || possible_moves;
  possible_moves = findDiagonRU(row, col) || possible_moves;
  return possible_moves;
}

/// @brief Answers weather the piece in given place belongs to current player's
/// turn
/// @param row Row coordinate
/// @param col Colum coordinate
bool Game::pieceTurn(int row, int col) {
  if ((board[row][col].getTileState() == WHITE && player_turn == WHITE) ||
      (board[row][col].getTileState() == BLACK && player_turn == BLACK)) {

    return true;
  }
  return false;
}

void Game::findWinner() {
  int white = 0, black = 0;
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      if (board[i][j].getTileState() == WHITE) {
        white++;
        continue;
      }
      if (board[i][j].getTileState() == BLACK) {
        black++;
        continue;
      }
    }
  }
  if (black == white) {
    printf("ITS A TIE!");
    return;
  }
  if (white > black) {
    printf("WHITE PLAYER WINS!");
  } else {
    printf("BLACK PLAYER WINS!");
  }
}
