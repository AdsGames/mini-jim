#ifndef TILE_H
#define TILE_H

#include <allegro.h>
#include <string>
#include <vector>

#include "TileType.h"

class Tile {
 public:
  explicit Tile(short type);
  Tile(short type, int x, int y);
  ~Tile();

  int getX() const;
  int getY() const;

  int getWidth() const;
  int getHeight() const;

  void setX(int x);
  void setY(int y);

  short getType() const;
  std::string getName() const;
  std::string getTypeStr() const;
  bool containsAttribute(int newAttribute);

  void setType(short type);
  void setType(std::string type);

  void draw(BITMAP* buffer, int xOffset, int yOffset, int frame);

 private:
  int x, y;
  std::vector<int> attribute;

  TileType* t_type;
};

#endif
