#pragma once

#include <asw/asw.h>
#include <string>
#include <vector>

#include "TileType.h"

class Tile {
 public:
  explicit Tile(short type);

  Tile(short type, int x, int y);

  void setX(int x);
  void setY(int y);

  asw::Quad<float> getTransform() const;

  short getType() const;
  std::string getName() const;
  std::string getTypeStr() const;
  bool containsAttribute(int newAttribute);

  void setType(short type);
  void setType(std::string type);

  void draw(int xOffset, int yOffset, int frame);

 private:
  asw::Vec2<float> position{0.0F, 0.0F};
  std::vector<int> attribute{};

  TileType* t_type{};
};
