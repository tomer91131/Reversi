#include "Tile.h"

void Tile::clearSources(){
    source.clear();
    state = EMPTY;
}
void Tile::addSource(int i, int j){
    source.push_back(Vector2i(i,j));
    state = VALID_MOVE;
}
void Tile::addSource(Vector2i vec){
    source.push_back(vec);
    state = VALID_MOVE;
}

Vector2i Tile::pollSource(){
    if(source.size() == 0){
        return Vector2i(-1,-1);
    }
    Vector2i vec(source.back());
    source.pop_back();
    return vec;
}

void Tile::setTileTexture(Texture &new_texture,state_t new_state){
    if(texture == nullptr){
        return;
    }else{
        this->setTexture(new_texture);
        state = new_state;
    }
}
state_t Tile::getTileState(){
    return this->state;
}

void Tile::setTileState(state_t new_state){
    this->state = new_state;
}