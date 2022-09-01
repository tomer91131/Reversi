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
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        board.push_back(vector<Tile>());
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            board[i].push_back(Tile());
            board[i][j].setPosition(100*j,100*i);
            board[i][j].setTileState(EMPTY);
            
        }
    }
    board[3][3].setTileTexture(white_pawn_texture,WHITE);
    board[3][4].setTileTexture(black_pawn_texture,BLACK);
    board[4][4].setTileTexture(white_pawn_texture,WHITE);
    board[4][3].setTileTexture(black_pawn_texture,BLACK);
}

void Game::updateWindow(){
    window->clear();
    window->draw(board_sprite);
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if(board[i][j].getTileState() != EMPTY){
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
            if(board[tile.x][tile.y].getTileState() == VALID_MOVE){
                makeMove(tile.x,tile.y);
                clearAvailableMoves();
                player_turn = (player_turn == WHITE)? BLACK:WHITE;
                if (!addPossibleMoves()){
                    printf("game ended!\n");
                    findWinner();
                }
                updateWindow();
            }
            break;
        }
    }
    return window->isOpen();
}

bool Game::pieceTurn(int row, int col){
    if((board[row][col].getTileState() == WHITE && player_turn == WHITE) ||
            (board[row][col].getTileState() == BLACK && player_turn == BLACK)){

        return true;
    }
    return false;
}

bool Game::findMoveUp(int row,int col){
    int i = row - 1,j = col;
    if (i <= 0 || !board[i][j].isCaptured() || pieceTurn(i,j)) {
        return false;
    }
    while (i > 0 && board[i][j].isCaptured() && !pieceTurn(i,j)){
        i--;
    }
    if (!board[i][j].isCaptured()){
        board[i][j].setTileTexture(valid_square_texture,VALID_MOVE);
        board[i][j].addSource(row,col);
        return true;
    }
    return false;
}
bool Game::findMoveDown(int row,int col){
    int i = row + 1,j = col;
    if(i >= FRAME || !board[i][j].isCaptured() || pieceTurn(i,j)){
        return false;
    }
    while(i < BOARD_SIZE && board[i][j].isCaptured() && !pieceTurn(i,j)){
        i++;
    }
    if(i < BOARD_SIZE && !board[i][j].isCaptured()){
        board[i][j].setTileTexture(valid_square_texture,VALID_MOVE);
        board[i][j].addSource(row,col);
        return true;
    }
    return false;
}
bool Game::findMoveRight(int row,int col){
    int i = row,j = col + 1;
    if (j >= FRAME || !board[i][j].isCaptured() || pieceTurn(i,j)){
        return false;
    }
    while (j < BOARD_SIZE && board[i][j].isCaptured() && !pieceTurn(i,j)){
        j++;
    }
    if (j < BOARD_SIZE && !board[i][j].isCaptured()){
        board[i][j].setTileTexture(valid_square_texture,VALID_MOVE);
        board[i][j].addSource(row,col);
        return true;
    }
    return false;
}
bool Game::findMoveLeft(int row,int col){
    int i = row,j = col - 1;
    if (j <= 0 || !board[i][j].isCaptured() || pieceTurn(i,j)){
        return false;
    }
    while (j > 0 && board[i][j].isCaptured() && !pieceTurn(i,j)){
        j--;
    }
    if (!board[i][j].isCaptured()){
        board[i][j].setTileTexture(valid_square_texture,VALID_MOVE);
        board[i][j].addSource(row,col);
        return true;
    }
    return false;
}
bool Game::findDiagonRU(int row,int col){
    int i = row - 1,j = col + 1;
    if (i <= 0 || j >= FRAME || !board[i][j].isCaptured() || pieceTurn(i,j)){
        return false;
    }
    while (i > 0 && j < BOARD_SIZE && board[i][j].isCaptured() && !pieceTurn(i,j)){
        j++;
        i--;
    }
    if (j < BOARD_SIZE && !board[i][j].isCaptured()){
        board[i][j].setTileTexture(valid_square_texture,VALID_MOVE);
        board[i][j].addSource(row,col);
        return true;
    }
    return false;
}
bool Game::findDiagonRD(int row,int col){
    int i = row + 1,j = col + 1;
    if (i >= FRAME || j >= FRAME || !board[i][j].isCaptured() || pieceTurn(i,j)){
        return false;
    }
    while (i < BOARD_SIZE && j < BOARD_SIZE && board[i][j].isCaptured() && !pieceTurn(i,j)){
        j++;
        i++;
    }
    if (i < BOARD_SIZE && j < BOARD_SIZE && !board[i][j].isCaptured()){
        board[i][j].setTileTexture(valid_square_texture,VALID_MOVE);
        board[i][j].addSource(row,col);
        return true;
    }
    return false;
}
bool Game::findDiagonLU(int row,int col){
    int i = row - 1,j = col - 1;
    if (i <= 0 || j <= 0 || !board[i][j].isCaptured() || pieceTurn(i,j)){
        return false;
    }
    while (i > 0 && j > 0 && board[i][j].isCaptured() && !pieceTurn(i,j)){
        j--;
        i--;
    }
    if (!board[i][j].isCaptured()){
        board[i][j].setTileTexture(valid_square_texture,VALID_MOVE);
        board[i][j].addSource(row,col);
        return true;
    }
    return false;
}
bool Game::findDiagonLD(int row,int col){
    int i = row + 1,j = col - 1;
    if (i >= FRAME || j <= 0 || !board[i][j].isCaptured() || pieceTurn(i,j)){
        return false;
    }
    while (i < BOARD_SIZE && j > 0 && board[i][j].isCaptured() && !pieceTurn(i,j)){
        j--;
        i++;
    }
    if (i < BOARD_SIZE && !board[i][j].isCaptured()){
        board[i][j].setTileTexture(valid_square_texture,VALID_MOVE);
        board[i][j].addSource(row,col);
        return true;
    }
    return false;
}

bool Game::findCapturable(int row,int col){
    bool possible_moves = false;
    possible_moves = findMoveUp(row,col) || possible_moves;
    possible_moves = findMoveDown(row,col) || possible_moves;
    possible_moves = findMoveRight(row,col) || possible_moves;
    possible_moves = findMoveLeft(row,col) || possible_moves;
    possible_moves = findDiagonLD(row,col) || possible_moves;
    possible_moves = findDiagonLU(row,col) || possible_moves;
    possible_moves = findDiagonRD(row,col) || possible_moves;
    possible_moves = findDiagonRU(row,col) || possible_moves;
    return possible_moves;
}

bool Game::addPossibleMoves(){
    bool possible_moves = false;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if(pieceTurn(i,j)){
                possible_moves = findCapturable(i,j) || possible_moves;
            }
        }
    }
    return possible_moves;
    
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
        while (i != row || j != col){
            changeTextureForPlayer(i,j);
            i += direction.x;
            j += direction.y;
        }
        source = board[row][col].pollSource();     
    }
    changeTextureForPlayer(row,col);
}
void Game::changeTextureForPlayer(int row, int col){
    if(player_turn == WHITE){
            board[row][col].setTileTexture(white_pawn_texture,WHITE);
        }else{
            board[row][col].setTileTexture(black_pawn_texture,BLACK);
        }
}
void Game::clearAvailableMoves(){
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if(board[i][j].getTileState() == VALID_MOVE){
                board[i][j].clearSources();
            }
        }
    }
}

Vector2i Game::getClickedTile() const{
    Vector2i vec(sf::Mouse::getPosition(*window));
    int tmp = vec.x;
    vec.x = vec.y / 100;
    vec.y = tmp / 100;
    return vec;
}

void Game::findWinner(){
    int white = 0,black = 0;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if(board[i][j].getTileState() == WHITE){
                white++;
                continue;
            }
            if(board[i][j].getTileState() == WHITE){
                black++;
                continue;
            }
        }
        
    }
    if(black == white){
        printf("ITS A TIE!");
        return;
    }
    if(white > black){
        printf("WHITE PLAYER WINS!");
    }else{
        printf("BLACK PLAYER WINS!");
    }
}
