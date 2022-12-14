#ifndef TILE_H
#define TILE_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>


using namespace std;
using namespace sf;

enum state_t { WHITE, BLACK, EMPTY, VALID_MOVE };

class Tile : public Sprite {
private:
  vector<Vector2i> source;
  state_t state = EMPTY;

public:
  Tile() = default;
  Tile(const Tile& other);
  ~Tile() = default;
  void clearSources();
  void addSource(int row, int col);
  void addSource(Vector2i location);
  Vector2i pollSource();
  void setTileTexture(Texture &new_texture, state_t new_state);
  state_t getTileState();
  void setTileState(state_t new_state);
  bool isCaptured();
};
#endif