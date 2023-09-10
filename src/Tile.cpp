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
auto Tile::getX() const -> int {
  if (t_type != nullptr) {
    return x + t_type->GetBBX();
  }

  return x;
}

auto Tile::getY() const -> int {
  if (t_type != nullptr) {
    return y + t_type->GetBBY();
  }

  return y;
}

auto Tile::getCenterX() const -> int {
  if (t_type != nullptr) {
    return x + t_type->GetBBX() + t_type->GetWidth() / 2;
  }

  return x;
}

auto Tile::getCenterY() const -> int {
  if (t_type != nullptr) {
    return y + t_type->GetBBY() + t_type->GetHeight() / 2;
  }

  return y;
}

auto Tile::getWidth() const -> int {
  if (t_type != nullptr) {
    return t_type->GetWidth();
  }

  return 0;
}

auto Tile::getHeight() const -> int {
  if (t_type != nullptr) {
    return t_type->GetHeight();
  }

  return 0;
}

void Tile::setX(int x) {
  this->x = x;
}

void Tile::setY(int y) {
  this->y = y;
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
    t_type->Draw(x - xOffset, y - yOffset, frame);
  }
}
