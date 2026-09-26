#include "Tile.h"

#include "globals.h"

#include "TileTypeLoader.h"

Tile::Tile(short type) : Tile(type, 0, 0) {}

Tile::Tile(short type, int x, int y) {
  setX(x);
  setY(y);
  setType(type);
}

asw::Quadf Tile::getTransform() const {
  if (t_type != nullptr) {
    return asw::Quadf(position + t_type->GetBoundingBox().position,
                      t_type->GetBoundingBox().size);
  }

  return asw::Quadf(0, 0, 0, 0);
}

void Tile::setX(int x) {
  this->position.x = x;
}

void Tile::setY(int y) {
  this->position.y = y;
}

TileType* Tile::getType() const {
  return t_type;
}

// Contains Attribute
auto Tile::containsAttribute(int newAttribute) -> bool {
  if (t_type != nullptr) {
    return t_type->HasAttribute(newAttribute);
  }

  return false;
}

void Tile::setType(std::string type) {
  t_type = TileTypeLoader::getTile(type);
}

// Set type
void Tile::setType(short type) {
  t_type = TileTypeLoader::getTile(type);
}

// Draw tile
void Tile::draw(int xOffset, int yOffset, int frame) {
  if (t_type != nullptr) {
    t_type->Draw(position.x - xOffset, position.y - yOffset, frame);
  }
}
