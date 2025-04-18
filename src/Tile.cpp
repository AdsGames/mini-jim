#include "Tile.h"

#include "globals.h"

#include "TileTypeLoader.h"

Tile::Tile(short type) : Tile(type, 0, 0) {}

Tile::Tile(short type, int x, int y) {
  setX(x);
  setY(y);
  setType(type);
}

asw::Quad<float> Tile::getTransform() const {
  if (t_type != nullptr) {
    return asw::Quad<float>(position + t_type->GetBoundingBox().position,
                            t_type->GetBoundingBox().size);
  }

  return asw::Quad<float>(0, 0, 0, 0);
}

void Tile::setX(int x) {
  this->position.x = x;
}

void Tile::setY(int y) {
  this->position.y = y;
}

auto Tile::getType() const -> short {
  if (t_type != nullptr) {
    return t_type->GetID();
  }

  return 0;
}

std::string Tile::getTypeStr() const {
  if (t_type != nullptr) {
    return t_type->GetIDStr();
  }

  return "";
}

std::string Tile::getName() const {
  if (t_type != nullptr) {
    return t_type->GetName();
  }

  return "";
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
