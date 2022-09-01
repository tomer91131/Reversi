#include "Tile.h"

void Tile::clearSources(){
    source.clear();
    state = EMPTY;
}
void Tile::addSource(int i, int j){
    printf("pushing source %d,%d\n",i,j);
    source.push_back(Vector2i(i,j));
}

void Tile::addSource(Vector2i vec){
    source.push_back(vec);
}

Vector2i Tile::pollSource(){
    printf("there are %d sources left\n",source.size());
    if(source.empty()){
        return Vector2i(-1,-1);
    }
    Vector2i vec(source.back());
    source.pop_back();
    return vec;
}

void Tile::setTileTexture(Texture &new_texture,state_t new_state){
        this->setTexture(new_texture);
        state = new_state;
}
state_t Tile::getTileState(){
    return this->state;
}

void Tile::setTileState(state_t new_state){
    this->state = new_state;
}

bool Tile::isCaptured(){
    return (state == WHITE || state == BLACK);
}