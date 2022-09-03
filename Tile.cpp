#include "Tile.h"

/// @brief Clear the locations of pawns that can move towards this tile
void Tile::clearSources() {
  source.clear();
  state = EMPTY;
}

/// @brief Add a location of a pawn, which can move towards this tile
/// @param row The row coordinate of the pawn
/// @param col The col coordinate of the pawn
void Tile::addSource(int row, int col) { source.push_back(Vector2i(row, col)); }

/// @brief Add a location of a pawn, which can move towards this tile
/// @param location The coordinates of the pawn 
void Tile::addSource(Vector2i location) { source.push_back(location); }

/// @brief Returns the location of one of the pawns, which can move to this tile.
/// @return The pawn's location if exsits, (-1,-1) if there are no locations left.
Vector2i Tile::pollSource() {
  if (source.empty()) {
    return Vector2i(-1, -1);
  }
  Vector2i vec(source.back());
  source.pop_back();
  return vec;
}
/// @brief Set the texture of the tile
/// @param new_texture A drawble texture
/// @param new_state The new state of the tile, according to board conventions
void Tile::setTileTexture(Texture &new_texture, state_t new_state) {
  this->setTexture(new_texture);
  state = new_state;
}
state_t Tile::getTileState() { return this->state; }

void Tile::setTileState(state_t new_state) { this->state = new_state; }

bool Tile::isCaptured() { return (state == WHITE || state == BLACK); }