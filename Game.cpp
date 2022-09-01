#include "Game.h"

Game::Game(){
    window = new RenderWindow(VideoMode(800,800),"Reversi");
    window->setFramerateLimit(4);
    if(!initImages()){
        printf("failed to load the images");
    }
    if(!initTextures()){
        printf("failed to load textures");
    }
    player_turn = WHITE;
    initBoard();

    addPossibleMoves();
    updateWindow();
}

Game::~Game(){
    delete window;
}

bool Game::initImages(){
    if (!(board_image.loadFromFile("images/board.jpg") 
            && black_pawn_image.loadFromFile("images/black_pawn.jpg")
            && white_pawn_image.loadFromFile("images/white_pawn.jpg") 
            && valid_square_image.loadFromFile("images/valid_square.jpg"))){
        return false;
    }
    return true;
}

bool Game::initTextures(){
    if (!(board_texture.loadFromImage(board_image) 
          && white_pawn_texture.loadFromImage(white_pawn_image)
          && black_pawn_texture.loadFromImage(black_pawn_image)
          && valid_square_texture.loadFromImage(valid_square_image))){
            return false;
    }
    board_sprite.setTexture(board_texture);
    return true;
}

void Game::initBoard(){
    checkpoint("initializing board\n")
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        board.push_back(vector<Tile>());
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            board[i].push_back(Tile());
            board[i][j].setPosition(100*j,100*i);
            
        }
    }
    checkpoint("success\nsetting first pawns\n")
    board[3][3].setTexture(white_pawn_texture);
    board[3][4].setTexture(black_pawn_texture);
    board[4][4].setTexture(white_pawn_texture);
    board[4][3].setTexture(black_pawn_texture);
    checkpoint("success\n")
}

void Game::updateWindow(){
    window->clear();
    window->draw(board_sprite);
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if(board[i][j].getTexture() != nullptr){
                window->draw(board[i][j]);
            }
        }
    }
    window->display();
}

bool Game::running(){
    while (window->pollEvent(event)){
        switch(event.type){
        case Event::Closed :
            printf("closing");
            window->close();
            break;
        case Event::MouseButtonPressed :
            Vector2i tile(getClickedTile());
            printf("clicked %d, %d",tile.x,tile.y);
            if(board[tile.x][tile.y].getTexture() == &valid_square_texture){
                makeMove(tile.x,tile.y);
                clearAvailableMoves();
                player_turn = (player_turn == WHITE)? BLACK:WHITE;
                addPossibleMoves();
                updateWindow();
            }
            break;
        }
    }
    return window->isOpen();
}

bool Game::pieceTurn(int row, int col){
    if((board[row][col].getTexture() == &white_pawn_texture && player_turn == WHITE) ||
            (board[row][col].getTexture() == &black_pawn_texture && player_turn == BLACK)){

        return true;
    }
    return false;
}

bool Game::findMoveUp(int row,int col){
    int i = row - 1,j = col;
    if (i == 0 || board[i][j].getTexture() == nullptr || pieceTurn(i,j) ) return false;
    while (i > 0 && board[i][j].getTexture() != nullptr && !pieceTurn(i,j)){
        i--;
    }
    if (board[i][j].getTexture() == nullptr){
        board[i][j].setTexture(valid_square_texture);
        board[i][j].addSource(row,col);
        return true;
    }
    return false;
}
bool Game::findMoveDown(int row,int col){
    int i = row + 1,j = col;
    if(i == BOARD_SIZE || board[i][j].getTexture() == nullptr || pieceTurn(i,j))return false;
    while(i < BOARD_SIZE && board[i][j].getTexture() != nullptr && !pieceTurn(i,j)){
        i++;
    }
    if(i < BOARD_SIZE && board[i][j].getTexture() == nullptr){
        board[i][j].setTexture(valid_square_texture);
        board[i][j].addSource(i,j);
        return true;
    }
    return false;
}
bool Game::findMoveRight(int row,int col){
    int i = row,j = col + 1;
    if (j == BOARD_SIZE || board[i][j].getTexture() == nullptr || pieceTurn(i,j))return false;
    while (j < BOARD_SIZE && board[i][j].getTexture() != nullptr && !pieceTurn(i,j)){
        j++;
    }
    if (j < BOARD_SIZE && board[i][j].getTexture() == nullptr){
        board[i][j].setTexture(valid_square_texture);
        board[i][j].addSource(i,j);
        return true;
    }
    return false;
}
bool Game::findMoveLeft(int row,int col){
    int i = row,j = col - 1;
    if (j == 0 || board[i][j].getTexture() == nullptr || pieceTurn(i,j))return false;
    while (j > 0 && board[i][j].getTexture() != nullptr && !pieceTurn(i,j)){
        j--;
    }
    if (board[i][j].getTexture() == nullptr){
        board[i][j].setTexture(valid_square_texture);
        board[i][j].addSource(i,j);
        return true;
    }
    return false;
}
bool Game::findDiagonRU(int row,int col){
    int i = row - 1,j = col + 1;
    if (i == 0 || j == BOARD_SIZE - 1 || board[i][j].getTexture() == nullptr || pieceTurn(i,j))return false;
    while (i > 0 || j < BOARD_SIZE && board[i][j].getTexture() != nullptr && !pieceTurn(i,j)){
        j++;
        i--;
    }
    if (j < BOARD_SIZE && board[i][j].getTexture() == nullptr){
        board[i][j].setTexture(valid_square_texture);
        board[i][j].addSource(i,j);
        return true;
    }
    return false;
}
bool Game::findDiagonRD(int row,int col){
    int i = row + 1,j = col + 1;
    if (i == BOARD_SIZE - 1 || j == BOARD_SIZE - 1 || board[i][j].getTexture() == nullptr || pieceTurn(i,j))return false;
    while (i < BOARD_SIZE || j < BOARD_SIZE && board[i][j].getTexture() != nullptr && !pieceTurn(i,j)){
        j++;
        i++;
    }
    if (i < BOARD_SIZE && j < BOARD_SIZE && board[i][j].getTexture() == nullptr){
        board[i][j].setTexture(valid_square_texture);
        board[i][j].addSource(i,j);
        return true;
    }
    return false;
}
bool Game::findDiagonLU(int row,int col){
    int i = row - 1,j = col - 1;
    if (i == 0 || j == 0 || board[i][j].getTexture() == nullptr || pieceTurn(i,j))return false;
    while (i > 0 || j > 0 && board[i][j].getTexture() != nullptr && !pieceTurn(i,j)){
        j--;
        i--;
    }
    if (board[i][j].getTexture() == nullptr){
        board[i][j].setTexture(valid_square_texture);
        board[i][j].addSource(i,j);
        return true;
    }
    return false;
}
bool Game::findDiagonLD(int row,int col){
    int i = row + 1,j = col - 1;
    if (i == 0 || j == 0 || board[i][j].getTexture() == nullptr || pieceTurn(i,j))return false;
    while (i > 0 || j > 0 && board[i][j].getTexture() != nullptr && !pieceTurn(i,j)){
        j--;
        i++;
    }
    if (i < BOARD_SIZE && board[i][j].getTexture() == nullptr){
        board[i][j].setTexture(valid_square_texture);
        board[i][j].addSource(i,j);
        return true;
    }
    return false;
}

void Game::findCapturable(int row,int col){
    int possible_moves = 0;
    possible_moves += findMoveUp(row,col);
    possible_moves += findMoveDown(row,col);
    possible_moves += findMoveRight(row,col);
    possible_moves += findMoveLeft(row,col);
    possible_moves += findDiagonLD(row,col);
    possible_moves += findDiagonLU(row,col);
    possible_moves += findDiagonRD(row,col);
    possible_moves += findDiagonRU(row,col);
    if(possible_moves == 0){
        window->close();
    }
}

bool Game::addPossibleMoves(){
    int possible_moves = 0;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if(pieceTurn(i,j)){
                findCapturable(i,j);
            }
        }
    }
    return true;
    
}

Vector2i Game::getDirection(int src_i, int src_j, int dst_i, int dst_j){
    Vector2i vec(0,0);
    if (src_i < dst_i){
        vec.x = 1;
    }else if (src_i > dst_i){
        vec.x = -1;
    }
    if (src_j < dst_j){
        vec.y = 1;
    }else if (src_j > dst_j){
        vec.y = -1;
    }
    return vec;
}

void Game::makeMove(int row,int col){
    Vector2i source = board[row][col].pollSource();
    while(source.x != -1){
        Vector2i direction(getDirection(source.x, source.y, row, col));
        int i = source.x + direction.x,j = source.y + direction.y;
        while (i != row && j != col){
            if(player_turn == WHITE){
                board[i][j].setTexture(white_pawn_texture);
            }else{
                board[i][j].setTexture(black_pawn_texture);
            }
            i += direction.x;
            j += direction.y;
        }
        source = board[row][col].pollSource();     
    }
}

void Game::clearAvailableMoves(){
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if(board[i][j].getTexture() == &valid_square_texture){
                board[i][j].setTexture()//need to change everything - solution is tile state field.
                board[i][j].clearSources();
            }
        }
    }
}

Vector2i Game::getClickedTile() const{
    Vector2i vec(sf::Mouse::getPosition(*window));
    vec.x = vec.x / 8;
    vec.y = vec.y / 8;
    return vec;
}
