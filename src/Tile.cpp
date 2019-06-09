#include "Tile.h"

#include "globals.h"
#include "utility/tools.h"

#include "TileTypeLoader.h"

tile::tile (short type) {
  setType (type);
}

tile::tile (short type, int x, int y) {
  setX (x);
  setY (y);
  setType (type);
}

tile::~tile() {

}

//Getters/ setters
int tile::getX() const {
  if (t_type)
    return x + t_type -> GetBBX();

  return x;
}

int tile::getY() const {
  if (t_type)
    return y + t_type -> GetBBY();

  return y;
}

int tile::getWidth() {
  if (t_type)
    return t_type -> GetWidth();

  return 0;
}

int tile::getHeight() {
  if (t_type)
    return t_type -> GetHeight();

  return 0;
}

void tile::setX (int x) {
  this -> x = x;
}

void tile::setY (int y) {
  this -> y = y;
}

short tile::getType() {
  if (t_type)
    return t_type -> GetID();

  return 0;
}

std::string tile::getTypeStr() {
  if (t_type)
    return t_type -> GetIDStr();

  return "";
}

std::string tile::getName() {
  if (t_type)
    return t_type -> GetName();

  return "";
}

// Contains Attribute
bool tile::containsAttribute (int newAttribute) {
  if (t_type)
    return t_type -> HasAttribute(newAttribute);

  return false;
}

void tile::setType (std::string type) {
  t_type = TileTypeLoader::GetTile(type);
}

// Set type
void tile::setType (short type) {
  t_type = TileTypeLoader::GetTile(type);
}

// Draw tile
void tile::draw_tile (BITMAP *buffer, int xOffset, int yOffset, int frame) {
  if (t_type)
    t_type -> Draw(buffer, x - xOffset, y - yOffset, frame);
}
