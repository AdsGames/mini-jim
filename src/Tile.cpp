#include "Tile.h"

#include "globals.h"
#include "utility/tools.h"

#include "TileTypeLoader.h"

Tile::Tile(short type) : Tile(type, 0, 0) {}

Tile::Tile(short type, int x, int y) {
  setX(x);
  setY(y);
  setType(type);
}

// Getters/ setters
int Tile::getX() const {
  if (t_type)
    return x + t_type->GetBBX();

  return x;
}

int Tile::getY() const {
  if (t_type)
    return y + t_type->GetBBY();

  return y;
}

int Tile::getWidth() const {
  if (t_type)
    return t_type->GetWidth();

  return 0;
}

int Tile::getHeight() const {
  if (t_type)
    return t_type->GetHeight();

  return 0;
}

void Tile::setX(int x) {
  this->x = x;
}

void Tile::setY(int y) {
  this->y = y;
}

short Tile::getType() const {
  if (t_type)
    return t_type->GetID();

  return 0;
}

std::string Tile::getTypeStr() const {
  if (t_type)
    return t_type->GetIDStr();

  return "";
}

std::string Tile::getName() const {
  if (t_type)
    return t_type->GetName();

  return "";
}

// Contains Attribute
bool Tile::containsAttribute(int newAttribute) {
  if (t_type)
    return t_type->HasAttribute(newAttribute);

  return false;
}

void Tile::setType(std::string type) {
  t_type = TileTypeLoader::GetTile(type);
}

// Set type
void Tile::setType(short type) {
  t_type = TileTypeLoader::GetTile(type);
}

// Draw tile
void Tile::draw(int xOffset, int yOffset, int frame) {
  if (t_type) {
    t_type->Draw(x - xOffset, y - yOffset, frame);
  }
}
