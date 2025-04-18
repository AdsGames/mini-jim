#pragma once

#include <asw/asw.h>
#include <string>
#include <vector>

#include "TileType.h"

class Tile {
 public:
  explicit Tile(short type);

  Tile(short type, int x, int y);

  int getX() const;
  int getY() const;

  int getCenterX() const;
  int getCenterY() const;

  int getWidth() const;
  int getHeight() const;

  void setX(int x);
  void setY(int y);

  asw::Quad<float> getBoundingBox() const;

  short getType() const;
  std::string getName() const;
  std::string getTypeStr() const;
  bool containsAttribute(int newAttribute);

  void setType(short type);
  void setType(std::string type);

  void draw(int xOffset, int yOffset, int frame);

 private:
  int x{}, y{};
  std::vector<int> attribute{};

  TileType* t_type{};
};
